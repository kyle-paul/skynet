#ifndef SKYNET_GUI_H
#define SKYNET_GUI_H

#include "core.h"
#include "data.h"
#include "util.h"
#include "imgui.h"
#include "scene.h"
#include "frame.h"
#include "texture2d.h"
#include "guizmo.h"
#include "imguizmo.h"
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
    void toolbar();

public:
    bool viewport_hover = false;
    uint32_t viewport_width, viewport_height;

private:
    ref<FrameBuffer> framebuffer = cref<FrameBuffer>();
    ref<Scene> scene;
    Guizmo guizmo;
    Serializer serial;
    Action action;

    ref<Texture2D> iconPlay;
    ref<Texture2D> iconEdit;
};

#endif // SKYNET_GUI_H