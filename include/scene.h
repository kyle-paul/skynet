#pragma once
#include "core.h"
#include "data.h"
#include "camera.h"
#include "shader.h"
#include "buffer.h"
#include "link.h"
#include "color.h"

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

private:
	Camera camera;
	ref<Shader> shader;

	std::unordered_map<std::string, ref<Link>> links;

	float light[3] = { -30.0f, -50.0f, 50.0f };
	float color[3] = {1.0f, 1.0f, 1.0f};
};