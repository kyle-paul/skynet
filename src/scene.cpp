#include "scene.h"
#include "math.h"
#include "imgui.h"
#include "imguizmo.h"

Scene::Scene() { }
Scene::~Scene() { }

std::mutex outMutex;
std::mutex meshMutex;

void loadMesh(const std::string &name, const MeshData &data, std::unordered_map<std::string, ref<Link>> &links) {
    try {
        ref<Mesh> mesh = cref<Mesh>(data.path, Object::Mesh, data.color, Mesh::Loader::scratch);
        { std::lock_guard<std::mutex> lock(meshMutex);
          links[name]->meshes.push_back(mesh); }

    } catch (const std::exception &e) {
        std::lock_guard<std::mutex> lock(outMutex);
        ERROR("Failed to load mesh -> {0}", e.what());
    }
}

void Scene::init() {
	base = std::filesystem::current_path().parent_path().string();
	shader = cref<Shader>("scene", base + "/assets/glsl/scene.glsl");
}

void Scene::load(const std::string &path) {
	xml::setData(&graph, &joints, &links, &u, &m, &dof_parent);
	xml::parseXML(path);

	std::vector<std::thread> threads;

	for (auto &[name, link] : this->links) {
		for (auto &d: link->data) {
			threads.emplace_back(loadMesh, name, d, std::ref(links));
		}
	}
	
	for (auto &thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    for (auto &[name, link] : this->links) {
		for (auto &mesh : link->meshes) {
			mesh->initGL();
		}
	}
	this->loaded = true;
	this->robotpath = path;
}

bool Scene::create(const Object &type, const std::string &name) {
	float color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	
	switch (type) {
		case(Object::Cube) : {

			if (objects.find(name) != objects.end()) {
				WARN("object name already exists");
				return false;
			}

			objects[name] = cref<Mesh>();
			objects[name]->genCube(0.25, 0.25, 0.25, color);
			objects[name]->initGL(); return true;
		}

		case(Object::Sphere) : {

			if (objects.find(name) != objects.end()) {
				WARN("object name already exists");
				return false;
			}

			objects[name] = cref<Mesh>();
			objects[name]->genSphere(0.3, 32, 32, color);
			objects[name]->initGL(); return true;
		}

		case (Object::Camera) : {

			if (objects.find(name) != objects.end()) {
				WARN("object name already exists");
				return false;
			}

			objects[name] = cref<Mesh>();
			objects[name]->genCamera(0.15, 0.15, 0.15, color);
			objects[name]->initGL();
			cams.push_back(name);
			return true;
		}
	}
	ASSERT(false, "Object type not found"); return false;
}

void Scene::render() {

	shader->bind();
	shader->setFloat3("light", light);
	
	switch(state) {
		case(SceneState::Edit) : {
			this->updateCamera();
			shader->setMat4("projview", camera.getProjView());
			shader->setFloat3("viewPos", camera.p);
			this->visualize();
			this->collision();
			break;
		}

		case(SceneState::Play) : {
			shader->setMat4("projview", camera.getProjView());
			shader->setFloat3("viewPos", camera.p);
			this->visualize();
			this->updatePhysics();
			break;
		}
	}

	if (this->loaded) { 
		this->forward("link0", "null"); 
	}

	shader->unbind();
}


void Scene::forward(const std::string &cur, const std::string &par) {
	float T[16], T_link[16], T_joint[16];

	if (par != "null") {
		math::matmul4(T_link, links[par]->getWorldTransform(), links[cur]->getLocalTransform());

		if (links[cur]->joint) {
			math::matmul4(T, T_link, links[cur]->joint->getJointTransform());
			links[cur]->joint->updateWorldTransform(T, links[cur]->p_w);
		} else {
			std::copy(T_link, T_link + 16, T);
		}
		links[cur]->setWorldTransform(T);
		links[cur]->render(shader);
	} else {
		links[cur]->setWorldTransform();
		links[cur]->render(shader);
	}

	for (auto &next : graph[cur]) {
		this->forward(next, cur);
	}
}

void Scene::compute_dof() {

	this->forward("link0", "null");
	this->forward("link0", "null");
	this->subtree("link0", "null");

	for (auto &[name, joint] : this->joints) {
		switch(joint->type) {
			case(JType::Hinge) : {
				float r[3], v[3];
				math::subVec3(r, this->subcom["link0"], joint->p_w);
				math::cross3(v, joint->w_w, r);
				math::copy3(joint->cdof, joint->w_w);
				math::copy3(joint->cdof+3, v);
				break;
			}
			case(JType::Slide) : {
				math::copy3(joint->cdof+3, joint->w_w);
				break;
			}
		}
	}
}

void Scene::subtree(const std::string &cur, const std::string &par) {

	this->m[cur] = links[cur]->mass;
	math::mulVecS3(this->u[cur], links[cur]->com_w, links[cur]->mass);

	for (auto &next : graph[cur]) {
		this->subtree(next, cur);
		this->m[cur] += this->m[next];
		math::addVec3(this->u[cur], this->u[cur], this->u[next]);
	}

	math::divVecS3(this->subcom[cur], this->u[cur], this->m[cur]);
}

void Scene::reset() {
	for (auto &[name, joint] : joints) {
		joint->a = 0.0f;
	}
}

void Scene::updatePhysics() {
	
	if (objects.find("sphere1") != objects.end()) {

		for (auto &[name, object] : objects) {

			if(name == "ground" || object->getType() == Object::Camera) continue;
			
			math::addScl3(object->vel, this->gravity, deltaTime);
			math::addScl3(object->vel, object->acc, deltaTime);
			math::addScl3(object->p, object->vel, deltaTime);

			if (object->p[2] <= objects["ground"]->p[2]) {
				object->p[2] = objects["ground"]->p[2];
				object->vel[2] *= -restitution;
			}
		}
	}

	this->collision();
}

void Scene::collision() {
	for (auto it1 = objects.begin(); it1 != objects.end(); ++it1) {
        auto &[name1, obj1] = *it1;

        // Ensure the object is a sphere
        if (obj1->getType() != Object::Sphere) continue;

        for (auto it2 = std::next(it1); it2 != objects.end(); ++it2) {
            auto &[name2, obj2] = *it2;

            // Ensure the second object is also a sphere
            if (obj2->getType() != Object::Sphere) continue;

            // Compute distance between centers
			float distance[3];
			math::subVec3(distance, obj1->p, obj2->p);

			float radiusSum = 0.3 + 0.3;
			float normDist = math::normVec3(distance);

            // Check for collision
            if (normDist <= radiusSum) {

				float depth = radiusSum - normDist;
				depth *= 0.5;

				if (normDist > 0.0f) { 

					float force[3];
					math::mulVecS3(force, distance, depth);
					math::addVec3(obj1->p, obj1->p, force);
					math::subVec3(obj2->p, obj2->p, force);
                }
			}
		}
	}
}

void Scene::inverse() {

	this->compute_dof();

	float error[3], offset[3], temp[3], joint_jac[3];
	float state_goal[3] = {this->objects["cube"]->p[0], this->objects["cube"]->p[1], this->objects["cube"]->p[2]};
	
	std::string end_effector = "left_finger";
	std::string closest_joint = "finger_joint1";

	float cur_state[3] = {links[end_effector]->p_w[0], links[end_effector]->p_w[1], links[end_effector]->p_w[2]};
	math::subVec3(error, state_goal, cur_state);

	int n = 3; int m = joints.size();

	int count = 0;
	float tolerance = 0.01;
	float step_size = 0.5;

	while (math::normVec(error, 3) > tolerance) {

		float jac[n * m]; std::fill(jac, jac + (n * m), 0.0f);
		math::subVec3(offset, state_goal, this->subcom["link0"]);
		
		std::string name = closest_joint;

		while(dof_parent[name].size() > 0) {
			auto &joint = joints[name];
			uint32_t i = joint->index;
			math::cross3(temp, joint->cdof, offset);
			math::addVec3(joint_jac, joint->cdof + 3, temp);
					
			jac[i + 0*m] = joint_jac[0];
			jac[i + 1*m] = joint_jac[1];
			jac[i + 2*m] = joint_jac[2];

			name = dof_parent[name];
		}

		float jac_T[m * n], j_inv[n * m], product[n * n], delta[m];
		
		math::transpose(jac_T, jac, n, m);
		math::matmul(product, jac, jac_T, n, m, n);
		math::invert(product, n);
		math::matmul(j_inv, jac_T, product, m, n, n);
		math::matmul(delta, j_inv, error, m, n, 1);

		name = closest_joint;

		while(dof_parent[name].size() > 0) {
			uint32_t i = joints[name]->index;
			joints[name]->a += step_size * delta[i];
			name = dof_parent[name];
		}

		this->compute_dof();

		cur_state[0] = links[end_effector]->p_w[0]; 
		cur_state[1] = links[end_effector]->p_w[1];
		cur_state[2] = links[end_effector]->p_w[2];

		math::subVec3(error, state_goal, cur_state);
		count++;
	}

	WARN("Done with {} loops", count);
}

void Scene::visualize() {
	for (auto &[name, object] : objects) {
		float* T = object->getLocalTransform(RotType::Euler);
		shader->setMat4("model", T);
		object->render(shader);
	}
}

void Scene::updateCamera() {
	if (!data->msc.first_left) {
		this->camera.e[1] += 0.01f * data->msc.dx;
		this->camera.e[0] += 0.01f * data->msc.dy;
	} 
	else if (!data->msc.first_right) {
		this->camera.p[0] -= 0.01f * data->msc.dx;
		this->camera.p[1] += 0.01f * data->msc.dy;
	}
	this->camera.p[2] += -0.1f * data->msc.zoom;

	data->msc.dx = 0.0f; 
	data->msc.dy = 0.0f; 
	data->msc.zoom = 0.0f;

	camera.updateProjView();
}

void Scene::editGuizmo() {
	if (data->opt.guizmo_local && selectedEntity) {
		float* T = selectedEntity->getWorldTransform();
		ImGuizmo::Manipulate(camera.getView(), camera.getProjection(), 
		(ImGuizmo::OPERATION)data->opt.guizmo_type, ImGuizmo::LOCAL, T);
		if (ImGuizmo::IsUsing) math::decompose(T, selectedEntity->p, selectedEntity->s, selectedEntity->e);
	}
}