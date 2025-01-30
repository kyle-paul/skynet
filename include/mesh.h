#ifndef SKYNET_MESH_H
#define SKYNET_MESH_H

#include "core.h"
#include "buffer.h"
#include "transformable.h"

class Mesh : public Transformable
{
public:
	enum class Loader {
		scratch = 0,
		assimp  = 1,
		tinyobj = 2,
	};

public:
	Mesh(const std::string &path, const Object &type, const float (&color)[4], const Loader& loader);
	~Mesh();
	
	void render(const ref<Shader> &shader);
	void initGL();
	void info();

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