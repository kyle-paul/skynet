#include "link.h"

Link::Link(float p[3], float q[4], float &mass, float* com, std::vector<MeshData> &data, const ref<Joint> &joint) {
	this->data = data;
	this->joint = joint;
	this->mass = mass;
	std::copy(p, p + 3, this->p);
    std::copy(q, q + 4, this->q);
	std::copy(com, com + 3, this->com);
}

Link::~Link() {
	
}

void Link::setWorldTransform(float* Tw) {
	if (Tw) {
		std::copy(Tw, Tw + 16, T);
		math::decomposeTw(T, p_w, q_w);
	}
	math::mulMatVec4(com_w, T, com);
}


void Link::render(const ref<Shader> &shader) {
	shader->setMat4("model", this->getWorldTransform());
	for (auto &mesh : meshes) {
		mesh->render(shader);
	}
}