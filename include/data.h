#pragma once

struct MouseConfig {
    float lastx = 0.0f, lasty = 0.0f;
    float dx = 0.0f, dy = 0.0f;
    float zoom = 0.0f;
    bool first_left = true;
    bool first_right = true;
    bool viewport_hover = false;
    float aspect = 1500.0f / 900.0f;
};

struct Option {
    bool guizmo_local = false;
    bool guizmo_world = true;
    int guizmo_type;
};

struct Data {
    MouseConfig msc;
    Option opt;
};

enum class RotType { Euler, Quaternion, Axis };
enum class RotAxis { Xaxis, Yaxis, Zaxis, Waxis };
enum class Frame { World, Body };
enum class Object { Cube, Sphere, Capsule, Tetrahedra };