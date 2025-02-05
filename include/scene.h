#ifndef SKYNET_SCENE_H
#define SKYNET_SCENE_H

#include "core.h"
#include "xml.h"
#include "engine.h"
#include "data.h"
#include "camera.h"
#include "shader.h"
#include "buffer.h"

class Scene 
{
public:
	Scene();
	~Scene();

	void init();
	void reset();
	void load(const std::string &path);
	void render();
	void updateCamera();
	void editGuizmo();
	inline void updateData(Data* data) { this->data = data; }

private:
	void visualize();
	void forward(const std::string &cur, const std::string &par);
	void subtree(const std::string &cur, const std::string &par);
	void compute_dof();
	void inverse();
	bool create(const Object &type, const std::string &name);
	void updatePhysics();
	void collision();

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
	std::unordered_map<std::string, float> m;
	std::unordered_map<std::string, float[3]> u;
	std::unordered_map<std::string, float[3]> subcom;
	std::unordered_map<std::string, std::string> dof_parent;
	std::vector<std::string> cams;

	std::vector<std::string> joint_names = {"joint1", "joint2", "joint3", "joint4", "joint5", "joint6", "joint7", "finger_joint1", "finger_joint2"};

	bool loaded = false;
	ref<Mesh> selectedEntity = nullptr;
	SceneState state = SceneState::Edit;
	uint32_t currentCamera = 0;

	float light[3] = { -30.0f, -50.0f, 50.0f };
	float bgcol[4] = { 0.15f, 0.15f, 0.15f, 1.0f };

	float gravity[3] = {0.0f, 0.0f, -9.81f};
	float deltaTime = 0.016f;
	float groundLevel = 0.0f;
	float restitution = 0.4f;

	friend class Interface;
	friend class Serializer;
};

#endif // SKYNET_SCENE_H