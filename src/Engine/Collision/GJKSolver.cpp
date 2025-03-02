#include "GJKSolver.h"
#include "Titan.hpp"
#include "BVHData.h"

namespace Skynet
{
    struct Simplex
    {
        int count;
        titan::vec3 A;
        titan::vec3 B;
        titan::vec3 C;
        titan::vec3 D;
    };

    titan::vec3 GetSupport(AABB& box, RigidBody& body, const titan::vec3& direction)
    {
        list<titan::vec3> corners = body.GetTransform() * box.GetCorners();
        titan::vec3 furthest = corners[0];
        titan::real best = corners[0] * direction;

        for(titan::vec3& corner : corners)
        {
            titan::real cur = corner * direction;
            if (cur > best)
            {
                furthest = corner;
                best = cur;
            }
        }
        return furthest;
    }

    void SolveSimplexLine(Simplex* simplex, titan::vec3& direction)
    {
        titan::vec3 AB = simplex->B - simplex->A;
        titan::vec3 AO = -simplex->A;

        direction = AB ^ AO ^ AB;
    }

    void SolveSimplexTriangle(PointCloud& cloud, Simplex* simplex, titan::vec3& direction)
    {
        titan::vec3 AB = simplex->B - simplex->A;
        titan::vec3 AC = simplex->C - simplex->A;
        titan::vec3 AO = -simplex->A;

        titan::vec3 ABC = AB ^ AC;
        titan::vec3 ABperp = AB ^ ABC;
        titan::vec3 ACperp = ABC ^ AC;

        if (ABperp * AO > 0)
        {
            cloud.Pop();
            simplex->count = 2;
            direction = AB ^ AO ^ AB;
        }
        else if (ACperp * AO > 0)
        {
            cloud.Pop();
            simplex->count = 2;
            direction = AC ^ AO ^ AC;
        }
        
        direction = ABC * AO > 0 ? ABC : -ABC;
    }

    bool SolveSimplexTetrahedron(PointCloud& cloud, Simplex* simplex, titan::vec3& direction)
    {
        titan::vec3 AB = simplex->B - simplex->A;
        titan::vec3 AC = simplex->C - simplex->A;
        titan::vec3 AD = simplex->D - simplex->A;
        titan::vec3 AO = -simplex->A;

        titan::vec3 ABC = AB ^ AC;
        titan::vec3 ACD = AC ^ AD;
        titan::vec3 ADB = AD ^ AB;

        if (ABC * AO > 0)
        {
            cloud.Pop();
            simplex->count = 3;
            direction = ABC;
            return false;
        }

        else if (ACD * AO > 0)
        {
            cloud.Pop();
            simplex->count = 3;
            direction = ACD;
            return false;
        }

        else if (ACD * AO > 0)
        {
            cloud.Pop();
            simplex->count = 3;
            direction = ACD;
            return false;
        }

        return true;
    }

    bool SimpleSolverGJK(PointCloud& cloud, AABB& box1, AABB& box2, RigidBody& body1, RigidBody& body2)
    {
        Simplex simplex;
        simplex.count = 1;

        titan::vec3 direction = body1.GetTransform() * box1.GetCenter() - body2.GetTransform() * box2.GetCenter();
        simplex.A = GetSupport(box2, body2, direction) - GetSupport(box1, body1, -direction); cloud.Add(simplex.A);
        direction = -simplex.A;

        int count = 1;
        bool collide = false;

        for (int i=0; i < 20; i++) // max iteration if two objects do not collide
        {
            simplex.count++;

            simplex.D = simplex.C;
            simplex.C = simplex.B;
            simplex.B = simplex.A;

            simplex.A = GetSupport(box2, body2, direction) - GetSupport(box1, body1, -direction); cloud.Add(simplex.A);
            if (simplex.A * direction < 0) return false;

            switch(simplex.count)
            {
                case(2) : SolveSimplexLine(&simplex, direction);     break;
                case(3) : SolveSimplexTriangle(cloud, &simplex, direction); break;
                case(4) : {
                    collide = SolveSimplexTetrahedron(cloud, &simplex, direction);
                    if (collide) return true;  break;
                }
                ASSERT(false, "Invalid simplex dimension in GJK algorithm!");
                return false;
            }
        }

        return false;
    }

    bool GJKBruteForce(PointCloud& cloud, AABB& boxA, AABB& boxB, RigidBody& bodyA, RigidBody& bodyB)
    {	
        list<titan::vec3> cornersA = bodyA.GetTransform() * boxA.GetCorners();
        list<titan::vec3> cornersB = bodyB.GetTransform() * boxB.GetCorners();

        for (titan::vec3& cA : cornersA)
        {
            for (titan::vec3& cB : cornersB)
            {
                titan::vec3 point = cA - cB;
                cloud.Add(point);
            }
        }

        return false;
    }
    
} // namespace Skynet
