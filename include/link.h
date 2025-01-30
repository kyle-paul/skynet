#ifndef SKYNET_LINK_H
#define SKYNET_LINK_H

#include "mesh.h"
#include "shader.h"
#include "joint.h"
#include "transformable.h"

struct MeshData {
    std::string path;
    float color[4];

    MeshData(const std::string & p, const float c[4]) : path(p) {
        std::copy(c, c + 4, color);
    }
};

struct Inertial {
    const float mass;
    const std::array<float, 3> pos;
    const std::array<float, 6> fullinertia;

    Inertial(const float mass_, const std::array<float, 3>& pos_, const std::array<float, 6>& fullinertia_) 
        : mass(mass_), pos(pos_), fullinertia(fullinertia_)
    {
    }
};

class Link : public Transformable
{
public:
	Link(float p[3], float q[4], std::vector<MeshData> &data, const ref<Joint> &joint=nullptr);
	~Link();

	void render(const ref<Shader> &shader);

public:
    ref<Joint> joint = nullptr;
	std::vector<MeshData> data;
	std::vector<ref<Mesh>> meshes;
};

#endif // SKYNET_LINK_H