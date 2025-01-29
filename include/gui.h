#pragma once

#include "core.h"
#include "data.h"
#include "imgui.h"
#include "scene.h"
#include "frame.h"
#include "guizmo.h"
#include "ImGuizmo.h"
#include "serializer.h"
#include <GLFW/glfw3.h>

class Interface
{
public:
    Interface(GLFWwindow *window);
    ~Interface();

    void render(ref<Scene> &scene);
    void begin();
    void end();

private:
    void styling();
    void menubar();

public:
    bool viewport_hover = false;
    uint32_t viewport_width, viewport_height;

private:
    ref<FrameBuffer> framebuffer = cref<FrameBuffer>();
    ref<Scene> scene;
    Guizmo guizmo;
    Serializer serial;
    Action action;
    bool dockspace_open = true;
};