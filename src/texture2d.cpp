#include "texture2d.h"
#include "glad/glad.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

Texture2D::Texture2D(uint32_t width, uint32_t height) : width(width), height(height) {
    internalFormat = GL_RGBA8;
    dataFormat = GL_RGBA;

    glCreateTextures(GL_TEXTURE_2D, 1, &textureID);
    glTextureStorage2D(textureID, 1, internalFormat, width, height);

    glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

Texture2D::Texture2D(const std::string &path) : path(path) {
    int width, height, channels;
    stbi_set_flip_vertically_on_load(1);
    stbi_uc *data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    ASSERT(data, "Fail to load image texture!");

    width = width;
    height = height;

    GLenum internalFormat = 0, dataFormat = 0;
    if (channels == 4) {
        internalFormat = GL_RGBA8;
        dataFormat = GL_RGBA;
    }
    else if (channels == 3) {
        internalFormat = GL_RGB8;
        dataFormat = GL_RGB;
    }

    internalFormat = internalFormat;
    dataFormat = dataFormat;

    ASSERT(internalFormat & dataFormat, "Format is not supported!")

    glCreateTextures(GL_TEXTURE_2D, 1, &textureID);
    glTextureStorage2D(textureID, 1, internalFormat, width, height);

    glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTextureSubImage2D(textureID, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
}

Texture2D::~Texture2D() {
    glDeleteTextures(1, &textureID);
}

void Texture2D::setData(void* data, uint32_t size) {
    uint32_t bpp = dataFormat == GL_RGBA ? 4 : 3;
    ASSERT(size == width * height * bpp, "Data must be entire texture!");
    glTextureSubImage2D(textureID, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);
}

void Texture2D::bind(uint32_t slot) const {
    glBindTextureUnit(slot, textureID);
}