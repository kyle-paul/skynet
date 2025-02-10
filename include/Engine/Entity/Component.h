#ifndef SKYNET_COMPONENT_H
#define SKYNET_COMPONENT_H

#include "Data.h"
#include "System.h"
#include "Mesh.h"
#include "Vector.h"
#include "RigidBody.h"


namespace Skynet
{

    struct LinkTransformComp
    {
        float p_local[3] = {0.0f, 0.0f, 0.0f};
        float q_local[4] = {0.0f, 0.0f, 0.0f, 0.0f};
        
        float p_world[3] = {0.0f, 0.0f, 0.0f};
        float q_world[4] = {0.0f, 0.0f, 0.0f, 0.0f};
        
        float s[3] = {0.0f, 0.0f, 0.0f};
        float T[16];

        LinkTransformComp() = default;
        LinkTransformComp(const LinkTransformComp&) = default;
        LinkTransformComp(const float* p) { std::copy(p, p+3, p_local); }

        inline void SetWorldTransform(float* T_world) {
            std::copy(T_world, T_world + 16, T);
        }

        inline void DecomposeTransform() {
            Math::DecomposeTw(T, p_world, q_world);
        }

        inline float* GetLocalTransform() {
            Math::Quat2T(T, q_local, p_local);
            return T;
        }

        inline float* GetWorldTransform() { return T; }
    };

    struct TransformComp
    {
        float p[3] = {0.0f, 0.0f, 0.0f};
        float e[3] = {0.0f, 0.0f, 0.0f};
        float s[3] = {1.0f, 1.0f, 1.0f};
        float q[4] = {1.0f, 0.0f, 0.0f, 0.0f};

        float T[16];

        TransformComp() = default;
        TransformComp(const TransformComp&) = default;
        TransformComp(const float* p, const float* e, const float* s, const float* q) 
        {
            std::copy(p, p+3, this->p); 
            std::copy(e, e+3, this->e); 
            std::copy(s, s+3, this->s); 
            std::copy(q, s+4, this->q); 
        }

        inline float* GetTransform(const RotType& type = RotType::Euler) 
        {
            float T_[16]; float R[16];
            Math::Transcale(T_, p, s);
            switch(type) {
                case(RotType::Quaternion) : Math::Quat2T(R, q);  break;
                case(RotType::Euler)      : Math::Euler2T(R, e); break;
            }
            Math::Matmul4(T, T_, R);
            return T;
        }
    };

    struct RigidBodyComp
    {
        RigidBody body;

        RigidBodyComp() = default;
        RigidBodyComp(const RigidBodyComp&) = default;
        RigidBodyComp(const float mass) {
            body.SetMass(mass);
            body.UpdateInertial(0.5f, 0.5f, 0.5f);
        }
    };

    struct MeshComp
    {
        ref<Mesh> mesh = cref<Mesh>();
        Object type;

        MeshComp() = default;
        MeshComp(const MeshComp&) = default;

        template<typename... Args>
        MeshComp(const Object& type, Args&&... args) : type(type)
        {
            switch(type)
            {
                case(Object::Mesh):
                    if constexpr (sizeof...(Args) == 2) {
                        mesh->GenMesh(std::forward<Args>(args)...);
                    } else {
                        ASSERT(false, "Invalid number of arguments for Mesh");
                    }
                    break;

                case(Object::Cube):
                    if constexpr (sizeof...(Args) == 3) {
                        mesh->GenCube(std::forward<Args>(args)...);
                    } else {
                        ASSERT(false, "Invalid number of arguments for Cube");
                    }
                    break;

                case(Object::Sphere):
                    if constexpr (sizeof...(Args) >= 1 && sizeof...(Args) <= 3) {
                        mesh->GenSphere(std::forward<Args>(args)...);
                    } else {
                        ASSERT(false, "Invalid number of arguments for Sphere");
                    }
                    break;

                default:
                    ASSERT(false, "Invalid object type");
            }
        }
    };

    struct VectorComp
    {
        Vector vector;

        VectorComp() = default;
        VectorComp(const VectorComp&) = default;
        VectorComp(float* start, float* end) { 
            vector.Submit(start, end);
            vector.InitGL();
        }
    };

    struct TextureComp
    {
        float color[4] = {1.0f, 1.0f, 1.0f, 1.0f};

        TextureComp() = default;
        TextureComp(const TextureComp&) = default;
        TextureComp(const float* c) { std::copy(c, c + 4, this->color); }
    };

    struct TagComp
    {
        std::string tag;

        TagComp() = default;
        TagComp(const TagComp&) = default;
        TagComp(const std::string &tag) : tag(tag) { }
    };

} // namespace Skynet


#endif // SKYNET_COMPONENT_H


