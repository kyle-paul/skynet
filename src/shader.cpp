#include "shader.h"

Shader::Shader(const std::string& name, const std::string& path) {
	std::string source = readfile(path);
    auto shaderSources = preprocess(source);    
    this->compile(shaderSources);
}

Shader::Shader(const std::string &name, const std::string& vertsrc, const std::string& fragsrc)
    :name(name)
{
    std::unordered_map<GLenum, std::string> sources;
    sources[GL_VERTEX_SHADER] = vertsrc;
    sources[GL_FRAGMENT_SHADER] = fragsrc;
    this->compile(sources);   
}

Shader::~Shader() {
    glDeleteProgram(shaderID);
}


std::string Shader::readfile(const std::string& path) {
    std::string result;
    std::fstream fin(path, std::ios::in | std::ios::binary);
    if (fin) {
        fin.seekg(0, std::ios::end);
        result.resize(fin.tellg());
        fin.seekg(0, std::ios::beg);
        fin.read(&result[0], result.size());
        fin.close();
    }
    else { ERROR("Could not open file path: {0}", path); }
    return result;
}

std::unordered_map<GLenum, std::string> Shader::preprocess(const std::string &source) {
    std::unordered_map<GLenum, std::string> shaderSources;
    const char* typeToken = "#type";
    size_t typeTokenLength = strlen(typeToken);
    size_t pos = source.find(typeToken, 0);

    while (pos != std::string::npos)
    {
        size_t eol = source.find_first_of("\r\n", pos);
        ASSERT(eol != std::string::npos, "Syntax error");
        size_t begin = pos + typeTokenLength + 1;
        std::string type = source.substr(begin, eol - begin);
        ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");
        size_t nextLinePos = source.find_first_not_of("\r\n", eol);
        pos = source.find(typeToken, nextLinePos);
        shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
    }
    return shaderSources;
}

void Shader::compile(const std::unordered_map<GLenum, std::string> &shaderSources) {

    GLuint program = glCreateProgram();
    ASSERT(shaderSources.size() <= 2, "Only support 2 shaders for now");

    std::array<GLenum, 2> glShaderIDs; 
    int glShaderIDIndex = 0;

    for (auto &kv : shaderSources) {
        GLenum type = kv.first;
        const std::string& source = kv.second;
        GLuint shader = glCreateShader(type);

        const GLchar* sourceCStr = source.c_str();
        glShaderSource(shader, 1, &sourceCStr, 0);
        glCompileShader(shader);

        GLint isCompiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

        if (isCompiled == GL_FALSE) {
            GLint maxLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);
            glDeleteShader(shader);

            ERROR("{0}", infoLog.data());
            WARN("Shader failed at {0}", glShaderIDIndex);
            ASSERT(false, "Shader compilation failure!");
            exit(-1);
        }
        glAttachShader(program, shader);
        glShaderIDs[glShaderIDIndex++] = shader;
    }

    shaderID = program;
    glLinkProgram(program);

    GLint isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
    
    if (isLinked == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
        
        glDeleteProgram(program);
        for (auto id : glShaderIDs) {
            glDeleteShader(id);
        }

        ERROR("{0}", infoLog.data());
        ASSERT(false, "Shader program link failure!");
        return;
    }

    for (auto id : glShaderIDs) {
        glDetachShader(program, id);
    }
}

void Shader::bind() const  {
    glUseProgram(shaderID);  
}

void Shader::unbind() const {
    glUseProgram(0);
}


void Shader::setInt(const std::string& name, int value) {
    GLint location = glGetUniformLocation(shaderID, name.c_str());
    glUniform1i(location, value);
}

void Shader::setIntArray(const std::string& name, int* values, uint32_t count) {
    GLint location = glGetUniformLocation(shaderID, name.c_str());
    glUniform1iv(location, count, values);
}

void Shader::setFloat(const std::string& name, float value) {
    GLint location = glGetUniformLocation(shaderID, name.c_str());
    glUniform1f(location, value);
}

void Shader::setFloat2(const std::string& name, const float* value) {
    GLint location = glGetUniformLocation(shaderID, name.c_str());
    glUniform2f(location, value[0], value[1]);
}

void Shader::setFloat3(const std::string& name, const float* value) {
    GLint location = glGetUniformLocation(shaderID, name.c_str());
    glUniform3f(location, value[0], value[1], value[2]);
}

void Shader::setFloat4(const std::string& name, const float* value) {
    GLint location = glGetUniformLocation(shaderID, name.c_str());
    glUniform4f(location, value[0], value[1], value[2], value[3]);
}

void Shader::setMat3(const std::string& name, const float* value) {
    GLint location = glGetUniformLocation(shaderID, name.c_str());
    glUniformMatrix3fv(location, 1, GL_FALSE, value);
}

void Shader::setMat4(const std::string& name, const float* value) {
    GLint location = glGetUniformLocation(shaderID, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, value);
}