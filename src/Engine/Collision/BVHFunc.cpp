#include "BVHFunc.h"
#include "Component.h"

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


        void BuildHierarchyTree(BVHNode*& node, const ref<Mesh>& mesh)
        {
            node = new BVHNode();
            node->box = AABB();

            for (int i = 0; i < mesh->faces.size(); i += 3)
            {
                Triangle tri;

                for (int j = 0; j < 3; j++)
                {
                    uint32_t idx = mesh->faces[i + j];
                    tri.points[j] = titan::vec3(mesh->verts[idx * 8 + 0], mesh->verts[idx * 8 + 1], mesh->verts[idx * 8 + 2]);
                }
                
                node->box.Expand(tri);
                node->triangles.push_back(tri);
            }
        }

        void HierarchySplit(BVHNode*& parent, int depth, int maxDepth)
        {
            if (parent == nullptr || depth >= maxDepth || parent->triangles.size() == 1) return;

            titan::vec3 size = parent->box.GetSize();
            int axis = size[0] > std::max(size[1], size[2]) ? 0 : size[1] > size[2] ? 1 : 2;
            float pos = parent->box.GetCenter()[axis]; 

            parent->childA = new BVHNode();
            parent->childB = new BVHNode();

            for(Triangle& tri : parent->triangles)
            {
                titan::vec3 center = tri.GetCenter();
                bool inA = center[axis] < pos;

                BVHNode* child = inA ? parent->childA : parent->childB;
                child->triangles.push_back(tri);
                child->box.Expand(tri);
            }

            HierarchySplit(parent->childA, depth + 1, maxDepth);
            HierarchySplit(parent->childB, depth + 1, maxDepth);
        }

        void UpdateBoundingBox(BVHNode*& parent, int depth, int maxDepth, const titan::mat4& T)
        {
            if (parent == nullptr || depth >= maxDepth) return;

            parent->box.min = T * parent->box.min;
            parent->box.max = T * parent->box.max;

            UpdateBoundingBox(parent->childA, depth + 1, maxDepth, T);
            UpdateBoundingBox(parent->childB, depth + 1, maxDepth, T);
        }

    } // namespace BVH

} // namespace Skynet
