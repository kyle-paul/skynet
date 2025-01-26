#pragma once

struct MouseConfig {
    float lastx = 0.0f, lasty = 0.0f;
    float dx = 0.0f, dy = 0.0f;
    float zoom = 0.0f;
    bool first = true;
};

enum class Rot { Xaxis, Yaxis, Zaxis, Waxis };
enum class Frame { World, Body };