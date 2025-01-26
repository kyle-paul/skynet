#include "scene.h"
#include "math.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

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

	links["link_0"] = cref<Link>(std::initializer_list<MeshData>{
		{"assets/mesh/panda/assets/link0_0.obj",  LIGHTWHITE},
		{"assets/mesh/panda/assets/link0_1.obj",  BLACK},
		{"assets/mesh/panda/assets/link0_2.obj",  LIGHTWHITE},
		{"assets/mesh/panda/assets/link0_3.obj",  BLACK},
		{"assets/mesh/panda/assets/link0_4.obj",  LIGHTWHITE},
		{"assets/mesh/panda/assets/link0_5.obj",  BLACK},
		{"assets/mesh/panda/assets/link0_6.obj",  WHITE},
		{"assets/mesh/panda/assets/link0_7.obj",  WHITE},
		{"assets/mesh/panda/assets/link0_8.obj",  BLACK},
		{"assets/mesh/panda/assets/link0_9.obj",  LIGHTWHITE},
		{"assets/mesh/panda/assets/link0_10.obj", WHITE}
	});

	links["link_1"] = cref<Link>(std::initializer_list<MeshData>{
		{"assets/mesh/panda/assets/link1.obj", WHITE}
	});

	links["link_2"] = cref<Link>(std::initializer_list<MeshData>{
		{"assets/mesh/panda/assets/link2.obj", WHITE}
	});

	links["link_3"] = cref<Link>(std::initializer_list<MeshData>{
		{"assets/mesh/panda/assets/link3_0.obj", WHITE},
	    {"assets/mesh/panda/assets/link3_0.obj", WHITE},
	    {"assets/mesh/panda/assets/link3_2.obj", WHITE},
	    {"assets/mesh/panda/assets/link3_3.obj", BLACK}
	});

	
	links["link_4"] = cref<Link>(std::initializer_list<MeshData>{
		{"assets/mesh/panda/assets/link4_0.obj", WHITE},
		{"assets/mesh/panda/assets/link4_1.obj", WHITE},
		{"assets/mesh/panda/assets/link4_2.obj", BLACK},
		{"assets/mesh/panda/assets/link4_3.obj", WHITE}
	});

	links["link_5"] = cref<Link>(std::initializer_list<MeshData>{
		{"assets/mesh/panda/assets/link5_0.obj", BLACK},
		{"assets/mesh/panda/assets/link5_1.obj", WHITE},
		{"assets/mesh/panda/assets/link5_2.obj", WHITE}
	});

	links["link_6"] = cref<Link>(std::initializer_list<MeshData>{
		{"assets/mesh/panda/assets/link6_0.obj", LIGHTWHITE},
	    {"assets/mesh/panda/assets/link6_1.obj", WHITE},
	    {"assets/mesh/panda/assets/link6_2.obj", BLACK},
	    {"assets/mesh/panda/assets/link6_3.obj", WHITE},
	    {"assets/mesh/panda/assets/link6_4.obj", WHITE},
	    {"assets/mesh/panda/assets/link6_5.obj", WHITE},
	    {"assets/mesh/panda/assets/link6_6.obj", WHITE},
	    {"assets/mesh/panda/assets/link6_7.obj", LIGHTBLUE},
	    {"assets/mesh/panda/assets/link6_8.obj", LIGHTBLUE},
	    {"assets/mesh/panda/assets/link6_9.obj", BLACK},
	    {"assets/mesh/panda/assets/link6_10.obj", BLACK},
	    {"assets/mesh/panda/assets/link6_11.obj", WHITE},
	    {"assets/mesh/panda/assets/link6_12.obj", GREEN},
	    {"assets/mesh/panda/assets/link6_13.obj", WHITE},
	    {"assets/mesh/panda/assets/link6_14.obj", BLACK},
	    {"assets/mesh/panda/assets/link6_15.obj", BLACK},
	    {"assets/mesh/panda/assets/link6_16.obj", WHITE}
	});

	links["link_7"] = cref<Link>(std::initializer_list<MeshData>{
		{"assets/mesh/panda/assets/link7_0.obj", WHITE},
	    {"assets/mesh/panda/assets/link7_1.obj", BLACK},
	    {"assets/mesh/panda/assets/link7_2.obj", BLACK},
	    {"assets/mesh/panda/assets/link7_3.obj", BLACK},
	    {"assets/mesh/panda/assets/link7_4.obj", BLACK},
	    {"assets/mesh/panda/assets/link7_5.obj", BLACK},
	    {"assets/mesh/panda/assets/link7_6.obj", BLACK},
	    {"assets/mesh/panda/assets/link7_7.obj", WHITE},
	});

	links["hand"] = cref<Link>(std::initializer_list<MeshData>{
		{"assets/mesh/panda/assets/hand_0.obj", LIGHTWHITE},
        {"assets/mesh/panda/assets/hand_1.obj", BLACK},
        {"assets/mesh/panda/assets/hand_2.obj", BLACK},
        {"assets/mesh/panda/assets/hand_3.obj", WHITE},
        {"assets/mesh/panda/assets/hand_4.obj", LIGHTWHITE}
	});

	links["finger_left"] = cref<Link>(std::initializer_list<MeshData>{
		{"assets/mesh/panda/assets/finger_0.obj", LIGHTWHITE},
		{"assets/mesh/panda/assets/finger_1.obj", BLACK}
	});

	links["finger_right"] = cref<Link>(std::initializer_list<MeshData>{
		{"assets/mesh/panda/assets/finger_0.obj", LIGHTWHITE},
		{"assets/mesh/panda/assets/finger_1.obj", BLACK}
	});

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
	this->multiMesh();
}

void Scene::render() {
	camera.updateProjView(); 
	float* projview = camera.getProjView();

	shader->bind();
	shader->setMat4("projview", projview);
	shader->setMat4("model", math::identity());
	shader->setFloat3("light", light);
	links["link_0"]->render(shader);
	shader->unbind();
}

void Scene::updateCamera(MouseConfig* msc) {
	this->camera.e[1] += 0.01f * msc->dx;
    this->camera.e[0] += 0.01f * msc->dy;
    this->camera.p[2] += -0.1f * msc->zoom;

    msc->dx = 0.0f; msc->dy = 0.0f; msc->zoom = 0.0f;
}