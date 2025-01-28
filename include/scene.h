#pragma once
#include "core.h"
#include "data.h"
#include "camera.h"
#include "shader.h"
#include "buffer.h"
#include "xml.h"

class Scene 
{
public:
	Scene();
	~Scene();

	void init();
	void render();
	void updateCamera();
	void editGuizmo();
	inline void updateData(Data* data) { this->data = data; }

private:
	void multiMesh();
	void visualize();
	void forward(const std::string &cur, const std::string &par);
	void inverse();
	void create(const Object &type);

private:
	Data* data;
	Camera camera;
	ref<Shader> shader;

	std::string base;
	std::unordered_map<std::string, ref<Link>> links;
	std::unordered_map<std::string, ref<Mesh>> objects;
	std::unordered_map<std::string, std::vector<std::string>> graph;
	ref<Mesh> selectedEntity = nullptr;

	float light[3] = { -30.0f, -50.0f, 50.0f };
	float bgcol[4] = { 0.15f, 0.15f, 0.15f, 1.0f};

	friend class Interface;
};