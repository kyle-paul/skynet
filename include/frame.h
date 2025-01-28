#pragma once

#include "core.h"
#include <glad/glad.h>

class FrameBuffer
{
public:
    FrameBuffer();
    ~FrameBuffer();

    void init();
    void bind() const;
    void unbind() const;

    void resize(uint32_t &newWidth, uint32_t &newHeight);

private:
    uint32_t fbo, color, depth;

    uint32_t width = 1500;
    uint32_t height = 900;

    friend class Interface;
};