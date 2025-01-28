#pragma once

#include "core.h"
#include "imgui.h"
#include "scene.h"
#include "frame.h"
#include <GLFW/glfw3.h>

class Interface
{
public:
    Interface(GLFWwindow *window);
    ~Interface();

    void styling();
    void render(Scene* scene);

    void begin();
    void end();

public:
    bool viewport_hover = false;
    uint32_t viewport_width, viewport_height;

private:
    ref<FrameBuffer> framebuffer = cref<FrameBuffer>();
};