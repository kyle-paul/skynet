#ifndef SKYNET_DATA_H
#define SKYNET_DATA_H

#include "core.h"

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
enum class Frame   { World, Body };
enum class Object  { Cube, Sphere, Capsule, Tetrahedra, Mesh };
enum class Action  { OpenFile, OpenScene, SaveScene };
enum class JType   { Hinge, Slide, Ball, Free};


inline static std::string ObjectToString(Object obj) {
    switch (obj) {
        case Object::Cube: return "Cube";
        case Object::Sphere: return "Sphere";
        case Object::Capsule: return "Capsule";
        case Object::Tetrahedra: return "Tetrahedra";
        case Object::Mesh: return "Mesh";
    }
    ASSERT(false, "Unknown object type");
}

inline static Object StringToObject(const std::string &str) {
    if (str == "Cube") return Object::Cube;
    if (str == "Sphere") return Object::Sphere;
    if (str == "Capsule") return Object::Capsule;
    if (str == "Tetrahedra") return Object::Tetrahedra;
    if (str == "Mesh") return Object::Mesh;
    ASSERT(false, "Unknown object type");
}

inline static std::string JTypeToString(JType type) {
    switch (type) {
        case JType::Hinge: return "hinge";
        case JType::Slide: return "slide";
        case JType::Ball: return "ball";
        case JType::Free: return "free";
    }
    ASSERT(false, "Unknown joint type");
}

inline static JType StringToJType(const std::string &str) {
    if (str == "hinge") return JType::Hinge;
    if (str == "slide") return JType::Slide;
    if (str == "ball") return JType::Ball;
    if (str == "free") return JType::Free;
    ASSERT(false, "Unknown joint type");
}

#endif // SKYNET_DATA_H