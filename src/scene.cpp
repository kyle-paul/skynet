#include "scene.h"

Scene::Scene() { }
Scene::~Scene() { }

void Scene::init() {
	shader = cref<Shader>("scene", "assets/glsl/scene.glsl");
}

void Scene::render() {
	float* view = camera.getView();
	float* proj = camera.getProjection();
}