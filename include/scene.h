#pragma once
#include "core.h"
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

private:
	Camera camera;
	ref<Shader> shader;
	ref<Mesh> cube;

	ref<VertexArray> va;
	ref<VertexBuffer> vb;
};