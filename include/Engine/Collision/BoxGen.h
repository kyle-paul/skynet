#ifndef SKYNET_COLLISION_BOXGEN
#define SKYNET_COLLISION_BOXGEN

#include "BVH.h"
#include "entt.hpp"
#include "Component.h"

namespace Skynet
{
    namespace BVH
    {
        static std::vector<std::pair<int, int>> edges = 
            {{0, 1}, {0, 2}, {0, 4}, {1, 3}, {1, 5}, {2, 3}, 
            {2, 6}, {3, 7}, {4, 5}, {4, 6}, {5, 7}, {6, 7}};

        void GenerateBox(entt::registry& vectors, BoundingBox* box)
        {

            std::vector<std::array<float, 3>> corners = box->GetCorners();

            for (auto& [u,v] : edges)
            {
                entt::entity e = vectors.create();
                vectors.emplace<VectorComp>(e, corners[u].data(), corners[v].data());
            }
        }
    }
}

#endif // SKYNET_COLLISION_BOXGEN