#include "Buffer.h"

namespace Skynet
{
    /* Vertex Array */
    VertexArray::VertexArray() {
        glCreateVertexArrays(1, &vao);
    }

    VertexArray::~VertexArray() {
        glDeleteVertexArrays(1, &vao);
    }

    void VertexArray::Bind() const {
        glBindVertexArray(vao);
    }

    void VertexArray::Unbind() const {
        glBindVertexArray(0);
    }

    const ref<VertexBuffer> VertexArray::GetVB() const {
        return this->vb;
    }

    const ref<IndexBuffer> VertexArray::GetIB() const {
        return this->ib;
    }

    void VertexArray::AddVB(const ref<VertexBuffer>& vb) {
        glBindVertexArray(vao);
        vb->Bind(); uint32_t index = 0;
        const auto &layout = vb->GetLayout();

        for (auto& element : layout) {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(
                index, element.GetDimCount(),
                ShaderDataTypeToOpenGL(element.type),
                element.normalized ? GL_TRUE : GL_FALSE,
                layout.getStride(),
                (const void*)(uintptr_t)element.offset
            );
            index++;
        }
    }

    void VertexArray::AddIB(const ref<IndexBuffer>& ib) {    
        glBindVertexArray(vao);
        ib->Bind(); this->ib = ib;
    }


    /* Vertex Buffer */
    VertexBuffer::VertexBuffer(float* vertices, uint32_t size) : size(size) {
        glCreateBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    }

    VertexBuffer::~VertexBuffer() {
        glDeleteBuffers(1, &vbo);
    }

    void VertexBuffer::Bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);	
    }

    void VertexBuffer::Unbind() const {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void VertexBuffer::SetData(const void* data, uint32_t size) {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }

    void VertexBuffer::SetLayout(const BufferLayout& layout) {
        this->layout = layout;
    }

    const BufferLayout& VertexBuffer::GetLayout() const {
        return this->layout;
    }


    /* Index Buffer */
    IndexBuffer::IndexBuffer(uint32_t *indices, uint32_t count) : count(count) {
        glCreateBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    }

    IndexBuffer::~IndexBuffer() {
        glDeleteBuffers(1, &ibo);
    }

    void IndexBuffer::Bind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    }

    void IndexBuffer::Unbind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

} // namespace Skynet
