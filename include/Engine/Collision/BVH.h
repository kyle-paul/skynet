#ifndef SKYNET_BOUNDING_H
#define SKYNET_BOUNDING_H

#include "RigidBody.h"

namespace Skynet
{
    class BoundingBox
    {
        
    };
    
    class OBB : public BoundingBox
    {
    public:
        OBB() { }
        ~OBB() { } 

        /* Control */ 
        void Merge() {

        }

        void Update() {

        }

        // /* Accessors */
        std::vector<std::array<float, 3>> GetCorners() const 
        {
            std::vector<std::array<float, 3>> corners;

            for (int i = 0; i < 8; ++i) {
                float signX = (i & 1) ? 1.0f : -1.0f;
                float signY = (i & 2) ? 1.0f : -1.0f;
                float signZ = (i & 4) ? 1.0f : -1.0f;

                float corner[3] = {
                    center[0] + signX * halfExtents[0] * rotation[0][0] +
                                signY * halfExtents[1] * rotation[0][1] +
                                signZ * halfExtents[2] * rotation[0][2],

                    center[1] + signX * halfExtents[0] * rotation[1][0] +
                                signY * halfExtents[1] * rotation[1][1] +
                                signZ * halfExtents[2] * rotation[1][2],

                    center[2] + signX * halfExtents[0] * rotation[2][0] +
                                signY * halfExtents[1] * rotation[2][1] +
                                signZ * halfExtents[2] * rotation[2][2]
                };

                corners.push_back({corner[0], corner[1], corner[2]});
            }
            return corners;
        }

        /* Interact */
        bool IsOverLaps(const BoundingBox* other) const {
            return false;
        }

    public:
        float center[3];
        float rotation[3][3];
        float halfExtents[3];
    };

    struct AABB : public BoundingBox
    {
        AABB();
        ~AABB();

    public:
        float min[3];
        float max[3];
    };

    struct Node
    {
        Node* children[2];
        BoundingBox box;

        RigidBody* body;

        bool IsLeaf() const
        {
            return body != NULL;
        }
    };

    
    class Hierarchy
    {

    };


} // namespace Skynet


#endif // SKYNET_BOUNDING_H