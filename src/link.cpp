#include "link.h"

Link::Link(std::initializer_list<MeshData> data) {
	this->data = data;
}

Link::~Link() {
	
}

void Link::render(const ref<Shader> &shader) {
	for (auto &mesh : meshes) {
		mesh->render(shader);
	}
}