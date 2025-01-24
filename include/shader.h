#pragma once
#include "core.h"
#include <glad/glad.h>


class Shader
{
public:
	Shader(const std::string& name, const std::string& path);
    Shader(const std::string& name, const std::string& vertsrc, const std::string& fragsrc);
	~Shader();

	void bind() const;
	void unbind() const;

	void setInt(const std::string& name, int value);
	void setIntArray(const std::string& name, int* value, uint32_t count);
	void setFloat(const std::string& name, float value);
    void setFloat2(const std::string& name, const float* value);
    void setFloat3(const std::string& name, const float* value);
    void setFloat4(const std::string& name, const float* value);
    void setMat3(const std::string& name, const float* value);
    void setMat4(const std::string& name, const float* value);

private:
    std::string readfile(const std::string& path);
    std::unordered_map<GLenum, std::string> preprocess(const std::string& source);
    void compile(const std::unordered_map<GLenum, std::string>& shaderSources);

    std::string name;
    uint32_t shaderID;
};


enum class ShaderDataType {
    None=0, Float, Float2, Float3, Float4, 
    Mat3, Mat4, Int, Int2, Int3, Int4, Bool
};

static uint32_t ShaderDataTypeSize(ShaderDataType type) {
    switch(type) {
        case ShaderDataType::Float:     return 4;           // 4 bytes
        case ShaderDataType::Float2:    return 4 * 2;       // 8 bytes
        case ShaderDataType::Float3:    return 4 * 3;       // 12 bytes
        case ShaderDataType::Float4:    return 4 * 4;       // 16 bytes
        case ShaderDataType::Mat3:      return 4 * 3 * 3;   // 36 bytes
        case ShaderDataType::Mat4:      return 4 * 4 * 4;   // 64 bytes
        case ShaderDataType::Int:       return 4;           // 4 bytes
        case ShaderDataType::Int2:      return 4 * 2;       // 8 bytes
        case ShaderDataType::Int3:      return 4 * 3;       // 12 bytes
        case ShaderDataType::Int4:      return 4 * 4;       // 16 bytes
        case ShaderDataType::Bool:      return 1;           // 1 byte
    }

    ASSERT(false, "Unknowned or Undefined ShaderDataType!");
    return 0;
};

static GLenum ShaderDataTypeToOpenGL(ShaderDataType type) {
    switch(type) {
        case ShaderDataType::Float  : return GL_FLOAT;
        case ShaderDataType::Float2 : return GL_FLOAT;
        case ShaderDataType::Float3 : return GL_FLOAT;
        case ShaderDataType::Float4 : return GL_FLOAT;
    }
    ASSERT(false, "Unknowned or Undefined type in ShaderDataTypeToOpenGL!");
    return 0;
}


static GLenum ShaderTypeFromString(const std::string type) {
    if (type == "vertex") {
        return GL_VERTEX_SHADER;
    }
    else if (type == "fragment" || type == "pixel") {
        return GL_FRAGMENT_SHADER;
    }
    ASSERT(false, "Unknown shader type!");
    return 0;
}