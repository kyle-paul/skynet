#include "scene.h"
#include "math.h"
#include "imgui.h"
#include "ImGuizmo.h"

Scene::Scene() { }
Scene::~Scene() { }

std::mutex outMutex;
std::mutex meshMutex;

void loadMesh(const std::string &name, const MeshData &data, std::unordered_map<std::string, ref<Link>> &links) {
    try {
        ref<Mesh> mesh = cref<Mesh>(data.path, data.color);
        { std::lock_guard<std::mutex> lock(meshMutex);
          links[name]->meshes.push_back(mesh); }

    } catch (const std::exception &e) {
        std::lock_guard<std::mutex> lock(outMutex);
        ERROR("Failed to load mesh -> {0}", e.what());
    }
}

void Scene::multiMesh() {

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
}

void Scene::init() {
	base = std::filesystem::current_path().string();
	shader = cref<Shader>("scene", base + "/assets/glsl/scene.glsl");
	xml::parseXML(base + "/assets/mesh/robots/panda/panda.xml", graph, links);
	this->multiMesh();
}

void Scene::create(const Object &type) {
	switch (type) {
		case(Object::Cube) : {
			float color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
			ref<Mesh> cube = cref<Mesh>(base + "/assets/mesh/primitive/cube.obj", color);
			cube->initGL(); this->objects["cube"] = cube;
			break;
		}
	}
}

void Scene::render() {
	this->updateCamera();
	float* projview = camera.getProjView();

	shader->bind();
	shader->setMat4("projview", projview);
	shader->setFloat3("light", light);
	this->forward("link0", "null");
	this->visualize();
	shader->unbind();
}


void Scene::forward(const std::string &cur, const std::string &par) {
	float T[16], T_link[16], T_joint[16];

	if (par != "null") {
		math::matmul4(T_link, links[par]->getWorldTransform(), links[cur]->getTransform());
		math::axis2T(T_joint, links[cur]->w, links[cur]->theta);
		math::matmul4(T, T_link, T_joint);

		links[cur]->setTransform(T_link);
		links[cur]->render(shader);

	} else {
		links[cur]->render(shader);
	}

	for (auto &next : graph[cur]) {
		this->forward(next, cur);
	}
}

void Scene::inverse() {
	
}

void Scene::visualize() {
	for (auto &[name, object] : objects) {
		float* T = object->getTransform(RotType::Euler);
		shader->setMat4("model", T);
		object->render(shader);
		this->selectedEntity = object;
	}
}

void Scene::updateCamera() {
	if (!data->msc.first_left) {
		this->camera.e[1] += 0.01f * data->msc.dx;
		this->camera.e[0] += 0.01f * data->msc.dy;		 
	} 
	else if (!data->msc.first_right) {
		this->camera.p[0] -= 0.001f * data->msc.dx;
		this->camera.p[1] += 0.001f * data->msc.dy;
	}
	this->camera.p[2] += -0.1f * data->msc.zoom;
	this->camera.aspect = data->msc.aspect;

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