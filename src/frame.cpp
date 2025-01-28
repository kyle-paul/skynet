#include "frame.h"

FrameBuffer::FrameBuffer() {
    this->init();   
}

FrameBuffer::~FrameBuffer() {
    glDeleteFramebuffers(1, &fbo);
}

void FrameBuffer::init() {
    glCreateFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // create texture color
    glCreateTextures(GL_TEXTURE_2D, 1, &color);
    glBindTexture(GL_TEXTURE_2D, color);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color, 0);

    // create a depth buffer
    glCreateTextures(GL_TEXTURE_2D, 1, &depth);
    glBindTexture(GL_TEXTURE_2D, depth);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depth, 0);
    ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void FrameBuffer::unbind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::resize(uint32_t &newWidth, uint32_t &newHeight) {
    this->width = newWidth;
    this->height = newHeight;

    glDeleteFramebuffers(1, &fbo);
    glDeleteTextures(1, &color);
    glDeleteTextures(1, &depth);

    this->init();
}