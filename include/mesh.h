#pragma once
#include "core.h"
#include "buffer.h"
#include "transformable.h"

class Mesh
{
public:
	enum class Loader {
		scratch = 0,
		assimp  = 1,
		tinyobj = 2,
	};

public:
	Mesh(const std::string &path, const float (&color)[4]);
	~Mesh();
	
	void load(const std::string &path);
	void render(const ref<Shader> &shader);
	void initGL();
	void info();

private:
	void readfile(const std::string& path);
	void assRead(const std::string& path);

	std::vector<std::string> split(const std::string &line,
			const std::string &del, std::vector<std::string> &res);

private:
	static Loader loader;
	std::vector<float> verts;
    std::vector<uint32_t> faces;
        
	ref<VertexArray>  va;
	ref<VertexBuffer> vb;
	ref<IndexBuffer>  ib;

	float color[4];
};