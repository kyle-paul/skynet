#ifndef SKYNET_SCENE_H
#define SKYNET_SCENE_H

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
	void load(const std::string &path);
	void render();
	void updateCamera();
	void editGuizmo();
	inline void updateData(Data* data) { this->data = data; }

private:
	void visualize();
	void forward(const std::string &cur, const std::string &par);
	void inverse();
	bool create(const Object &type, const std::string &name);

private:
	Data* data;
	Camera camera;
	ref<Shader> shader;

	std::string base;
	std::string robotpath;
	std::unordered_map<std::string, ref<Mesh>> objects;
	std::unordered_map<std::string, ref<Link>> links;
	std::unordered_map<std::string, ref<Joint>> joints;
	std::unordered_map<std::string, std::vector<std::string>> graph;

	bool loaded = false;
	ref<Mesh> selectedEntity = nullptr;

	float light[3] = { -30.0f, -50.0f, 50.0f };
	float bgcol[4] = { 0.15f, 0.15f, 0.15f, 1.0f};

	friend class Interface;
	friend class Serializer;
};

#endif // SKYNET_SCENE_H