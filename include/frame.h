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

    uint32_t width = 1200;
    uint32_t height = 800;

    friend class Interface;
};