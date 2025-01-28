#include "link.h"

Link::Link(float p[3], float q[4], float w[3], std::vector<MeshData> &data) {
	this->data = data;
	std::copy(p, p + 3, this->p);
    std::copy(q, q + 4, this->q);
    std::copy(w, w + 3, this->w);
}

Link::~Link() {
	
}

void Link::render(const ref<Shader> &shader) {
	shader->setMat4("model", this->getWorldTransform());
	for (auto &mesh : meshes) {
		mesh->render(shader);
	}
}