#ifndef SKYNET_COLLISION_GJK
#define SKYNET_COLLISION_GJK

#include "Component.h"

namespace Skynet
{
    bool GJKBruteForce(PointCloud& cloud, AABB& boxA, AABB& boxB, RigidBody& bodyA, RigidBody& bodyB);

    bool SimpleSolverGJK(PointCloud& cloud, AABB& box1, AABB& box2, RigidBody& body1, RigidBody& body2);

} // namespace Skynet


#endif // SKYNET_COLLISION_GJK