#ifndef SKYNET_MESH_H
#define SKYNET_MESH_H

#include "core.h"
#include "buffer.h"
#include "physics.h"
#include "transformable.h"

class Mesh : public Transformable, public Physics
{
public:
	enum class Loader {
		scratch = 0,
		assimp  = 1,
		tinyobj = 2,
	};

public:
	Mesh() = default;
	Mesh(const std::string &path, const Object &type, const float (&color)[4], const Loader& loader);
	~Mesh();
	
	void render(const ref<Shader> &shader);
	void initGL();
	void info();

	void genCube(float halfx, float halfy, float halfz, float* color);
	void genCamera(float halfx, float halfy, float halfz, float* color);
	void genSphere(float radius, int sectors, int stacks, float* color);

	inline Object getType() const { return type; }

private:
	void readfile(const std::string &path);
	void assRead(const std::string &path);

	std::vector<std::string> split(const std::string &line,
			const std::string &del, std::vector<std::string> &res);

private:
	std::vector<float> verts;
    std::vector<uint32_t> faces;
        
	ref<VertexArray>  va;
	ref<VertexBuffer> vb;
	ref<IndexBuffer>  ib;

	Object type;
	float color[4];

	friend class Interface;
	friend class Serializer;
};

#endif // SKYNET_MESH_H