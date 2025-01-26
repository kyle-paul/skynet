#include "scene.h"
#include "math.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

Scene::Scene() { }
Scene::~Scene() { }

void Scene::init() {
	shader = cref<Shader>("scene", "assets/glsl/scene.glsl");
	cube = cref<Mesh>("assets/mesh/panda/assets/link0_8.obj");
}

void Scene::render() {
	camera.updateProjView(); 
	float* projview = camera.getProjView();

	shader->bind();
	shader->setMat4("projview", projview);
	shader->setMat4("model", math::identity());
	shader->setFloat3("light", light);
	shader->setFloat3("color", color);
	cube->render();
	shader->unbind();
}

void Scene::updateCamera(MouseConfig* msc) {
	this->camera.e[1] += 0.01f * msc->dx;
    this->camera.e[0] += 0.01f * msc->dy;
    this->camera.p[2] += -0.1f * msc->zoom;

    msc->dx = 0.0f; msc->dy = 0.0f; msc->zoom = 0.0f;
}