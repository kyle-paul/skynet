#ifndef SKYNET_COMPONENT_H
#define SKYNET_COMPONENT_H

#include "Data.h"
#include "System.h"
#include "Mesh.h"
#include "Vector.h"
#include "RigidBody.h"
#include "BVHData.h"
#include "BVHFunc.h"
#include "Titan.hpp"
#include "Renderer.h"

namespace Skynet
{

    struct LinkTransformComp
    {
        titan::vec3 p_local;
        titan::quat q_local;

        titan::vec3 p_world;
        titan::quat q_world;

        titan::vec3 s = {1.0f, 1.0f, 1.0f};
        titan::mat4 T;

        LinkTransformComp() = default;
        LinkTransformComp(const LinkTransformComp&) = default;
        LinkTransformComp(titan::vec3 p) : p_local(p) {}

        inline void SetWorldTransform(titan::mat4& T_world) {
            T = T_world;
        }

        inline void DecomposeTransform() {}

        inline titan::mat4 GetLocalTransform() {
            return titan::Transcale(p_local, s) * q_local.Quat2T();
        }

        inline titan::mat4& GetWorldTransform() { return T; }
    };

    struct TransformComp
    {
        titan::vec3 p;
        titan::vec3 e;
        titan::vec3 s = {1.0f, 1.0f, 1.0f};
        titan::quat q;
        titan::mat4 T;

        TransformComp() = default;
        TransformComp(const TransformComp&) = default;
        TransformComp(titan::vec3 p, titan::vec3 e, titan::vec3 s, titan::quat q) 
            :p(p), e(e), s(s), q(q)
        {

        }

        inline titan::mat4 GetTransform(const RotType& type = RotType::Euler) 
        {
            titan::mat4 R;
            switch(type) {
                case(RotType::Quaternion) : R = q.Quat2T();  break;
                case(RotType::Euler)      : R = titan::Euler2T(this->e); break;
            }

            return titan::Transcale(this->p, this->s) * R;
        }
    };

    struct RigidBodyComp
    {
        RigidBody body;
        BodyType type;

        RigidBodyComp() = default;
        RigidBodyComp(const RigidBodyComp&) = default;
        RigidBodyComp(const BodyType& type, const float mass) {
            this->type = type;
            body.SetMass(mass);
            body.SetInertialTensor(0.5f, 0.5f, 0.5f);
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
                    if constexpr (sizeof...(Args) == 1) {
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

    static std::vector<std::pair<int, int>> edges =
    {
        {0, 1}, {1, 3}, {3, 2}, {2, 0},
        {4, 5}, {5, 7}, {7, 6}, {6, 4},
        {0, 4}, {1, 5}, {3, 7}, {2, 6}
    };

    struct BVHComp
    {
        BVHNode* node = nullptr;
        list<Vector> lines;

        int maxDepth = 1;
        titan::vec4 color = {1.0f, 0.0f, 0.0f, 1.0f};

        BVHComp() = default;
        BVHComp(const ref<Mesh>& mesh) {
            compute(mesh);
        }

        void compute(const ref<Mesh>& mesh)
        {
            if (node) {
                delete node;
                node = nullptr;
            }

            lines.clear();

            BVH::BuildHierarchyTree(node, mesh);
            BVH::HierarchySplit(node, 0, maxDepth);
            this->DrawNodes(node, 0);
        }

        void DrawNodes(BVHNode* node, int depth)
        {
            if (node == nullptr || depth >= maxDepth) return;

            list<titan::vec3> corners = node->box.GetCorners();

            for (auto& [start, end] : edges)
            {
                Vector line;
                line.Submit(corners[start].raw(), corners[end].raw());
                line.InitGL();
                lines.push_back(line);
            }

            DrawNodes(node->childA, depth + 1);
            DrawNodes(node->childB, depth + 1);
        }

        void Render(const ref<Shader>& shad) {
            for (Vector& line : lines) 
            {
                shad->SetFloat4("color", color.raw());
                Renderer::DrawLine(line.GetVA());
            }
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
        TagComp(const std::string& tag) : tag(tag) { }
    };

} // namespace Skynet


#endif // SKYNET_COMPONENT_H