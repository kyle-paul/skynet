#include "scene.h"
#include "math.h"

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
	shader = cref<Shader>("scene", "assets/glsl/scene.glsl");
	xml::parseXML("/home/paul/dev/graphics/assets/mesh/robots/panda/panda.xml", graph, links);
	this->multiMesh();
}

void Scene::render() {
	camera.updateProjView();
	float* projview = camera.getProjView();

	shader->bind();
	shader->setMat4("projview", projview);
	shader->setFloat3("light", light);
	this->forward("link0", "null");
	shader->unbind();
}

void Scene::forward(const std::string &cur, const std::string &par) {
	float T[16], T_link[16], T_joint[16];

	if (par != "null") {
		math::matmul4(T_link, links[par]->getWorldTransform(), links[cur]->getTransform());
		math::axis2R(T_joint, links[cur]->w, links[cur]->theta);
		math::matmul4(T, T_link, T_joint);

		links[cur]->setTransform(T);
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

void Scene::updateCamera(MouseConfig* msc) {
	if (!msc->first_left) {
		this->camera.e[1] += 0.01f * msc->dx;
		this->camera.e[0] += 0.01f * msc->dy;		 
	} 
	else if (!msc->first_right) {
		this->camera.p[0] -= 0.001f * msc->dx;
		this->camera.p[1] += 0.001f * msc->dy;
	}
	this->camera.p[2] += -0.1f * msc->zoom;
	this->camera.aspect = msc->aspect;
	msc->dx = 0.0f; msc->dy = 0.0f; msc->zoom = 0.0f;
}