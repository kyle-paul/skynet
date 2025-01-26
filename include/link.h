#pragma once
#include "mesh.h"
#include "shader.h"
#include "transformable.h"

struct MeshData {
    std::string path;
    float color[3];

    MeshData(const std::string& p, const float c[3]) : path(p) {
        std::copy(c, c + 3, color);
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

class Link : Transformable
{
public:
	Link(std::initializer_list<MeshData> data);
	~Link();

	void render(const ref<Shader> &shader);

public:
	std::vector<MeshData> data;
	std::vector<ref<Mesh>> meshes;
};