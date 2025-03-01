#ifndef SKYNET_BUFFER_H
#define SKYNET_BUFFER_H

#include "System.h"
#include "Shader.h"

namespace Skynet
{

    struct BufferElement 
    {
        std::string name;
        uint32_t size;
        uint32_t offset;

        ShaderDataType type;
        bool normalized;

        BufferElement(ShaderDataType type, const std::string& name, bool normalized=false)
        :name(name), type(type), size(ShaderDataTypeSize(type)), offset(0), normalized(normalized) { }

        uint32_t GetDimCount() const 
        {
            switch (type) 
            {
                case ShaderDataType::Float:  return 1;
                case ShaderDataType::Float2: return 2;
                case ShaderDataType::Float3: return 3;
                case ShaderDataType::Float4: return 4;
                case ShaderDataType::Mat3:   return 3;
                case ShaderDataType::Mat4:   return 4;
                case ShaderDataType::Int:    return 1;
                case ShaderDataType::Int2:   return 2;
                case ShaderDataType::Int3:   return 3;
                case ShaderDataType::Int4:   return 4;
                case ShaderDataType::Bool:   return 1;
            }
            ASSERT(false, "Unknowned or Undefined Dimension of Data!");
            return 0;
        }
    };

    class BufferLayout
    {
    public:
        BufferLayout() {}
        BufferLayout(const std::initializer_list<BufferElement>& layoutArray) : elements(layoutArray) 
        { 
            CalculateOffsetAndStride();
        }

        inline const std::vector<BufferElement>& getElements() const { return elements; }
        uint32_t getStride() const { return stride; }

        std::vector<BufferElement>::iterator begin() { return elements.begin(); }
        std::vector<BufferElement>::iterator end() { return elements.end(); }
        std::vector<BufferElement>::const_iterator begin() const { return elements.begin(); }
        std::vector<BufferElement>::const_iterator end() const { return elements.end(); }

    private:
        void CalculateOffsetAndStride() 
        {
            uint32_t offset = 0;
            stride = 0;
            for (auto &element : elements) 
            {
                element.offset = offset;
                offset += element.size;
                stride += element.size;
            }
        }
    private:
        std::vector<BufferElement> elements;
        uint32_t stride;
    };


    class VertexBuffer
    {
    public:
        VertexBuffer(float* vertices, uint32_t size);
        ~VertexBuffer();

        void Bind() const;
        void Unbind() const;

        void SetData(const void* data, uint32_t size);
        void SetLayout(const BufferLayout& layout);

        const BufferLayout& GetLayout() const;
        const uint32_t GetSize() const;

    private:
        uint32_t vbo;
        uint32_t size;
        BufferLayout layout;
    };


    class IndexBuffer
    {
    public:
        IndexBuffer(uint32_t* indices, uint32_t count);
        ~IndexBuffer();

        void Bind() const;
        void Unbind() const;

        inline uint32_t GetCount() const { return count; }	

    private:
        uint32_t ibo;
        uint32_t count;
    };


    class VertexArray
    {
    public:
        VertexArray();
        ~VertexArray();

        void Bind() const;
        void Unbind() const;

        void AddVB(const ref<VertexBuffer>& vb);
        void AddIB(const ref<IndexBuffer>& ib);

        const ref<VertexBuffer> GetVB() const;
        const ref<IndexBuffer> GetIB() const;

    private:
        uint32_t vao;
        ref<VertexBuffer> vb;
        ref<IndexBuffer> ib;
    };


} // namespace Skynet

#endif  // SKYNET_BUFFER_H


