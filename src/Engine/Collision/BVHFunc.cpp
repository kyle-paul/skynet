#include "BVHFunc.h"
#include "Component.h"
#include "Contact.h"

namespace Skynet
{
    namespace BVH
    {
        void FitCovariance(float* points, float& n, OBB* box) 
        {
            /* Step 1: Compute centroid */
            titan::vec3 centroid;

            for(int i = 0; i < n; i++)
            {
                centroid[0] += points[i * 8 + 0];
                centroid[1] += points[i * 8 + 1];
                centroid[2] += points[i * 8 + 2];
            }
            
            centroid = centroid * (1.0f / n);

            /* Step 2: Compute covariance matrix */
            titan::mat3 covariance;
            
            /*  Σ = 1/N SUM_1^N (pi - p)(pi - p)^T  */
            for(int i = 0; i < n; i++)
            {
                float x = points[i * 8 + 0] - centroid[0];
                float y = points[i * 8 + 1] - centroid[1];
                float z = points[i * 8 + 2] - centroid[2];

                covariance(0, 0) += x * x;
                covariance(0, 1) += x * y;
                covariance(0, 2) += x * z;

                covariance(1, 0) += y * x;
                covariance(1, 1) += y * y;
                covariance(1, 2) += y * z;

                covariance(2, 0) += z * x;
                covariance(2, 1) += z * y;
                covariance(2, 2) += z * z;
            }

            covariance = covariance * (1.0f / n);

            /* Step 3: Compute eigenvectors */
            titan::vec3 eigenvalues;
            titan::mat3 eigenvectors;

            titan::EigenDecompositionJacobi(covariance, eigenvalues, eigenvectors);

            /* Store rotation matrix (eigenvectors) */
            box->rotation = eigenvectors;

            /* Step 4: Transform vertices into this new space */
            titan::vec3 minBound(FLT_MAX, FLT_MAX, FLT_MAX);
            titan::vec3 maxBound(-FLT_MAX, -FLT_MAX, -FLT_MAX);

            for(int i = 0; i < n; i++) 
            {
                titan::vec3 temp(points[i * 8 + 0], points[i * 8 + 1], points[i * 8 + 2]);
                titan::vec3 proj = (temp - centroid) * eigenvectors;

                minBound = titan::MinVec3(minBound, proj);
                maxBound = titan::MaxVec3(maxBound, proj);
            }

            /* Step 5: Compute final OBB properties */
            box->center = centroid + eigenvectors * ((minBound + maxBound) * 0.5f);
            box->halfExtents = (maxBound - minBound) * 0.5f;
        }

        void BuildHierarchyTree(BVHNode*& root, const ref<Mesh>& mesh) {
            root = new BVHNode();
            root->box = AABB();
            mesh->StoreTriangles();
            auto& triangles = mesh->triangles; 
            for(size_t i = 0; i < triangles.size(); i++) {
                root->box.Expand(triangles[i]);
                root->triangles.push_back(i);
            }
        }

        void HierarchySplit(BVHNode*& node, int depth, int maxDepth, const ref<Mesh>& mesh) {
            if (node == nullptr || depth >= maxDepth || node->triangles.size() == 1) return;

            titan::vec3 size = node->box.GetSize();
            int axis = size[0] > std::max(size[1], size[2]) ? 0 : size[1] > size[2] ? 1 : 2;
            float pos = node->box.GetCenter()[axis]; 

            node->childA = new BVHNode();
            node->childB = new BVHNode();

            for(auto& index : node->triangles) {
                titan::vec3 center = mesh->triangles[index].GetCenter();
                bool inA = center[axis] < pos;

                BVHNode* child = inA ? node->childA : node->childB;
                child->triangles.push_back(index);
                child->box.Expand(mesh->triangles[index]);
            }

            HierarchySplit(node->childA, depth + 1, maxDepth, mesh);
            HierarchySplit(node->childB, depth + 1, maxDepth, mesh);
        }

        bool IsCollision(titan::vec3& min1, titan::vec3& max1, titan::vec3& min2, titan::vec3& max2) {
            return (min1[0] <= max2[0] && max1[0] >= min2[0]) &&
                   (min1[1] <= max2[1] && max1[1] >= min2[1]) &&
                   (min1[2] <= max2[2] && max1[2] >= min2[2]);
        }

        void CheckCollision(entt::registry& bodies, float& k, float& b) {
            size_t n = bodies.size();

            for (int i = 1; i < n; i++) {
                auto& bvhA  = bodies.get<BVHComp>((entt::entity)i);
                auto& bodyA = bodies.get<RigidBodyComp>((entt::entity)i).body;

                titan::vec3 min1 = bvhA.root->box.GetTransMin(bodyA.GetTransform());
                titan::vec3 max1 = bvhA.root->box.GetTransMax(bodyA.GetTransform());

                /* Check collision with other objects */ 
                for (int j = i + 1; j < n; j++) {
                    auto& bvhB  = bodies.get<BVHComp>((entt::entity)j);
                    auto& bodyB = bodies.get<RigidBodyComp>((entt::entity)j).body;

                    titan::vec3 min2 = bvhB.root->box.GetTransMin(bodyB.GetTransform());
                    titan::vec3 max2 = bvhB.root->box.GetTransMax(bodyB.GetTransform());

                    if (IsCollision(min1, max1, min2, max2)) {
                        INFO("Collision");
                    }
                }

                /* Check collision with default ground */ 
                auto& ground = bodies.get<BVHComp>((entt::entity)0).root->box;

                if (min1[1] < ground.max[1]) {
                    float depth = min1[1] - ground.max[1];
                    bodyA.force_int[1] = -k * depth - b * bodyA.vel[1] + bodyA.mass * 9.81;
                }
                else bodyA.force_int[1] = 0.0f;
            }
        }

    } // namespace BVH

} // namespace Skynet
