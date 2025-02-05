#ifndef SKYNET_TEXTURE_H
#define SKYNET_TEXTURE_H

#include "core.h"

class Texture
{
public:
    virtual ~Texture() = default;

    virtual uint32_t getWidth() const = 0;
    virtual uint32_t getHeight() const = 0;
    virtual uint32_t getTextureID() const = 0;

    virtual void setData(void* data, uint32_t size) = 0;
    virtual void bind(uint32_t slot = 0) const = 0;
    virtual bool isLoaded() const = 0;
    virtual std::string getPath() const = 0;
    virtual bool operator==(const Texture& other) const = 0;
};

class Texture2D : public Texture
{
public:
    Texture2D(uint32_t width, uint32_t height);
    Texture2D(const std::string &path);
    virtual ~Texture2D();
    
    virtual uint32_t getWidth() const override { return width; }
    virtual uint32_t getHeight() const override { return height; }
    virtual uint32_t getTextureID() const override {  return textureID; }

    virtual void setData(void* data, uint32_t size) override;
    virtual void bind(uint32_t slot = 0) const override;
    virtual bool isLoaded() const override { return loaded; }

    virtual std::string getPath() const override {
        if (path.size() > 0) return path;
        else return "white texture";
    }

    inline virtual bool operator==(const Texture &other) const override {
        return textureID == ((Texture2D&)other).textureID;
    }

private:
    std::string path;
    uint32_t width, height, textureID;
    uint32_t internalFormat, dataFormat;

    bool loaded = false;
};

#endif // SKYNET_TEXTURE_H