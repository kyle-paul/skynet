#ifndef SKYNET_ENUMS_H
#define SKYNET_ENUMS_H

#include "System.h"

namespace Skynet
{
    // enum class
    enum class RotType    { Euler, Quaternion, Axis };
    enum class RotAxis    { Xaxis, Yaxis, Zaxis, Waxis };
    enum class Reference  { World, Body };
    enum class Object     { Cube, Sphere, Capsule, Tetrahedra, Mesh, Camera};
    enum class Action     { OpenFile, OpenScene, SaveScene };
    enum class JType      { Hinge, Slide, Ball, Free};
    enum class SceneState { Edit, Play };
    enum class Loader     { Scratch, Assimp, Tinyobj };

    // util convert funcs
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

} // namespace Skynet

#endif // SKYNET_ENUMS_H