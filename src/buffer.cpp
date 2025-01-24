#include "buffer.h"

VertexArray::VertexArray() {
	glCreateVertexArrays(1, &vao);
}

VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &vao);
}

void VertexArray::bind() const {
	glBindVertexArray(vao);
}

void VertexArray::unbind() const {
    glBindVertexArray(0);
}

const ref<VertexBuffer> VertexArray::getVB() const {
	return this->vb;
}

const ref<IndexBuffer> VertexArray::getIB() const {
	return this->ib;
}

void VertexArray::addVB(const ref<VertexBuffer>& vb) {
	glBindVertexArray(vao);
	vb->bind(); uint32_t index = 0;
	const auto &layout = vb->getLayout();

	for (auto& element : layout) {
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(
			index, element.getDimCount(),
			ShaderDataTypeToOpenGL(element.type),
			element.normalized ? GL_TRUE : GL_FALSE,
			layout.getStride(),
			(const void*)(uintptr_t)element.offset
		);
		index++;
	}
}

void VertexArray::addIB(const ref<IndexBuffer>& ib) {    
    glBindVertexArray(vao);
    ib->bind(); this->ib = ib;
}

ref<VertexArray> VertexArray::create() {
	return cref<VertexArray>();
}


VertexBuffer::VertexBuffer(float* vertices, uint32_t& size) : size(size) {
	glCreateBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer() {
	glDeleteBuffers(1, &vbo);
}

void VertexBuffer::bind() const {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);	
}

void VertexBuffer::unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::setData(const void* data, uint32_t& size) {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

void VertexBuffer::setLayout(const BufferLayout& layout) {
	this->layout = layout;
}

const BufferLayout& VertexBuffer::getLayout() const {
	return this->layout;
}

ref<VertexBuffer> VertexBuffer::create(float* vertices, uint32_t& size) {
	return cref<VertexBuffer>(vertices, size);
}



IndexBuffer::IndexBuffer(uint32_t *indices, uint32_t& count) : count(count) {
	glCreateBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &ibo);
}

void IndexBuffer::bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
}

void IndexBuffer::unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

ref<IndexBuffer> IndexBuffer::create(uint32_t* indices, uint32_t& count) {
	return cref<IndexBuffer>(indices, count);
}