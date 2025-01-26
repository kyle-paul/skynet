#pragma once
#include "core.h"
#include "data.h"
#include "camera.h"
#include "shader.h"
#include "buffer.h"
#include "mesh.h"

class Scene 
{
public:
	Scene();
	~Scene();

	void init();
	void render();
	void updateCamera(MouseConfig* msc);

private:
	Camera camera;
	ref<Shader> shader;
	ref<Mesh> cube;

	float light[3] = { -30.0f, -50.0f, 50.0f };
	float color[3] = {1.0f, 1.0f, 1.0f};
};