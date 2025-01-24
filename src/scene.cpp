#include "scene.h"
#include "math.h"

Scene::Scene() { }
Scene::~Scene() { }

void Scene::init() {
	shader = cref<Shader>("scene", "assets/glsl/scene.glsl");
	cube = cref<Mesh>("assets/mesh/primitive/cube.obj");
}

void Scene::render() {
	camera.updateProjView(); 
	float* projview = camera.getProjView();

	shader->bind();
	shader->setMat4("projview", projview);
	cube->render();
	shader->unbind();
}