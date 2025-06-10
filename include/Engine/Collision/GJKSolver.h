#ifndef SKYNET_ENGINE_COLLISION_GJK
#define SKYNET_ENGINE_COLLISION_GJK

#include "Component.h"

namespace Skynet
{
    struct Simplex {
        int count;
        titan::vec3 A;
        titan::vec3 B;
        titan::vec3 C;
        titan::vec3 D;
    };

    struct Face {
        titan::vec3 a, b, c;
        titan::vec3 n;
        float d;

        Face(titan::vec3 a_, titan::vec3 b_, titan::vec3 c_) : a(a_), b(b_), c(c_) {
            titan::vec3 ab = b - a;
            titan::vec3 ac = c - a;
            n = (ab ^ ac).normalized();
            d = n * a;
            if (d < 0) {
                n = -n;
                d = -d;
            }
        }

        bool IsVisibleFrom(const titan::vec3& p) const {
            return (n * (p - a)) > 0;
        }
    };

    struct Edge {
        titan::vec3 a, b;
        bool operator<(const Edge& other) const {
            return true;
        }
    };

    void SimpleSolverEPA(Simplex* simplex, 
                         titan::vec3& ABC, titan::vec3& ACD, titan::vec3& ADB, titan::vec3& BDC,
                         AABB& box1, AABB& box2, RigidBody& body1, RigidBody& body2);
    bool GJKBruteForce(PointCloud& cloud, AABB& boxA, AABB& boxB, RigidBody& bodyA, RigidBody& bodyB);
    bool SimpleSolverGJK(PointCloud& cloud, AABB& box1, AABB& box2, RigidBody& body1, RigidBody& body2);

} // namespace Skynet


#endif // SKYNET_ENGINE_COLLISION_GJK