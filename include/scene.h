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
	void updateCamera(MouseConfig* msc);

private:
	void multiMesh();
	void forward(const std::string &cur, const std::string &par);
	void inverse();

private:
	Camera camera;
	ref<Shader> shader;

	std::unordered_map<std::string, ref<Link>> links;
	std::unordered_map<std::string, std::vector<std::string>> graph;

	float light[3] = { -30.0f, -50.0f, 50.0f };
	float color[3] = {1.0f, 1.0f, 1.0f};
};