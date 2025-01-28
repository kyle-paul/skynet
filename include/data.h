#pragma once

struct MouseConfig {
    float lastx = 0.0f, lasty = 0.0f;
    float dx = 0.0f, dy = 0.0f;
    float zoom = 0.0f;
    bool first_left = true;
    bool first_right = true;
    bool viewport_hover = false;
    float aspect = 1200.0f / 800.0f;
};

enum class Rot { Xaxis, Yaxis, Zaxis, Waxis };
enum class Frame { World, Body };