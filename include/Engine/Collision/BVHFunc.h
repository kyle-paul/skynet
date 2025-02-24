#ifndef SKYNET_COLLISION_BVHFUNC_H
#define SKYNET_COLLISION_BVHFUNC_H

#include "BVHData.h"
#include "entt.hpp"

namespace Skynet
{
    namespace BVH
    {
        void FitCovariance(float* points, float& n, OBB* box);

        void BuildHierarchyTree(BVHNode*& node, const ref<Mesh>& mesh);

        void HierarchySplit(BVHNode*& parent, int depth, int maxDepth);

        void UpdateBoundingBox(BVHNode*& parent, int depth, int maxDepth, const titan::mat4& T);

    } // namespace BVH

} // namespace Skynet

#endif // SKYNET_COLLISION_BVHFUNC_H