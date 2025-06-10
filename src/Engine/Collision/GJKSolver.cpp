#include "GJKSolver.h"
#include "Titan.hpp"
#include "BVHData.h"

namespace Skynet
{
    const static int EPS = -1e9;

    titan::vec3 GetSupport(AABB& box, RigidBody& body, const titan::vec3& direction) {
        list<titan::vec3> corners = body.GetTransform() * box.GetCorners();
        titan::vec3 furthest = corners[0];
        titan::real best = corners[0] * direction;

        for(titan::vec3& corner : corners) {
            titan::real cur = corner * direction;
            if (cur > best) {
                furthest = corner;
                best = cur;
            }
        }
        return furthest;
    }

    void SimpleSolverEPA(Simplex* simplex, AABB& box1, AABB& box2, RigidBody& body1, RigidBody& body2) {
        list<titan::vec3> polytope = { simplex->A, simplex->B, simplex->C, simplex->D };
        list<Face> faces = {
            Face(simplex->A, simplex->B, simplex->C), // ABC
            Face(simplex->A, simplex->C, simplex->D), // ACD
            Face(simplex->A, simplex->D, simplex->B), // ADB
            Face(simplex->B, simplex->D, simplex->C)  // BDC
        };

        const size_t maxIter = 32;  // Increased from 15
        const float EPS = 1e-6f;

        for(size_t i = 0; i < maxIter; i++) {
            // Find closest face
            auto closestIt = faces.begin();
            float minDist = closestIt->d;
            
            for(auto it = faces.begin(); it != faces.end(); ++it) {
                if(it->d < minDist) {
                    minDist = it->d;
                    closestIt = it;
                }
            }
            
            Face closestFace = *closestIt;
            titan::vec3 newPoint = GetSupport(box2, body2, closestFace.n) - 
                                   GetSupport(box1, body1, -closestFace.n);

            // Check convergence
            float penetration = (newPoint * closestFace.n) - closestFace.d;
            if (abs(penetration) < EPS * 10) {
                INFO("EPA Converged in {} iterations!", i);
                INFO("Normal: "); closestFace.n.log();
                INFO("Depth: {}", closestFace.d);
                return;
            }

            // Rebuild polytope
            std::set<Edge> uniqueEdges;
            for(auto it = faces.begin(); it != faces.end(); ) {
                if(it->IsVisibleFrom(newPoint)) {
                    uniqueEdges.insert({it->a, it->b});
                    uniqueEdges.insert({it->b, it->c});
                    uniqueEdges.insert({it->c, it->a});
                    it = faces.erase(it);
                } else {
                    ++it;
                }
            }

            // Add new faces
            for(const Edge& edge : uniqueEdges) {
                faces.emplace_back(edge.a, edge.b, newPoint);
            }

            if(faces.empty()) {
                INFO("EPA Failed - No faces remaining");
                return;
            }
        }
        INFO("EPA Failed to Converge in {} iterations!", maxIter);
    }

    void SolveSimplexLine(Simplex* simplex, titan::vec3& direction) {
        titan::vec3 AB = simplex->B - simplex->A;
        titan::vec3 AO = -simplex->A;
        direction = AB ^ AO ^ AB;
    }

    void SolveSimplexTriangle(PointCloud& cloud, Simplex* simplex, titan::vec3& direction) {
        titan::vec3 AB = simplex->B - simplex->A;
        titan::vec3 AC = simplex->C - simplex->A;
        titan::vec3 AO = -simplex->A;

        titan::vec3 ABC = AB ^ AC;
        titan::vec3 ABperp = AB ^ ABC;
        titan::vec3 ACperp = ABC ^ AC;

        if (ABperp * AO > 0){
            cloud.Pop();
            simplex->count = 2;
            direction = ABperp;
            return;
        }
        else if (ACperp * AO > 0) {
            cloud.Pop();
            simplex->count = 2;
            direction = ACperp;
            return;
        }
        
        else direction = ABC * AO > 0 ? ABC : -ABC;
    }

    bool SolveSimplexTetrahedron(PointCloud& cloud, Simplex* simplex, titan::vec3& direction,
                                 AABB& box1, AABB& box2, RigidBody& body1, RigidBody& body2) 
    {
        titan::vec3 AB = simplex->B - simplex->A;
        titan::vec3 AC = simplex->C - simplex->A;
        titan::vec3 AD = simplex->D - simplex->A;
        titan::vec3 AO = -simplex->A;

        titan::vec3 ABC = AB ^ AC;
        titan::vec3 ACD = AC ^ AD;
        titan::vec3 ADB = AD ^ AB;

        if (ABC * AO > 0) {
            cloud.Pop();
            simplex->count = 3;
            direction = ABC;
            return false;
        }

        else if (ACD * AO > 0) {
            cloud.Pop();
            simplex->count = 3;
            direction = ACD;
            return false;
        }

        else if (ADB * AO > 0) {
            cloud.Pop();
            simplex->count = 3;
            direction = ADB;
            return false;
        }

        else {
            SimpleSolverEPA(simplex, box1, box2, body1, body2);
            return true;
        }
    }


    bool SimpleSolverGJK(PointCloud& cloud, AABB& box1, AABB& box2, RigidBody& body1, RigidBody& body2) {
        Simplex simplex;
        simplex.count = 1;
        bool collide = false;

        titan::vec3 direction = body1.GetTransform() * box1.GetCenter() - body2.GetTransform() * box2.GetCenter();
        simplex.A = GetSupport(box2, body2, direction) - GetSupport(box1, body1, -direction); cloud.Add(simplex.A);
        direction = -simplex.A;

        // max iteration if two objects do not collide
        for (int i = 0; i < 20 && !collide; i++) {
            simplex.count++;

            simplex.D = simplex.C;
            simplex.C = simplex.B;
            simplex.B = simplex.A;

            simplex.A = GetSupport(box2, body2, direction) - GetSupport(box1, body1, -direction); cloud.Add(simplex.A);
            if (simplex.A * direction < 0) return false;

            switch(simplex.count)
            {
                case(2) : SolveSimplexLine(&simplex, direction); break;
                case(3) : SolveSimplexTriangle(cloud, &simplex, direction); break;
                case(4) : {
                    collide |= SolveSimplexTetrahedron(cloud, &simplex, direction, box1, box2, body1, body2);
                    if (collide) return true; break;
                }
                ASSERT(false, "Invalid simplex dimension in GJK algorithm!");
                return false;
            }
        }

        return collide;
    }

    bool GJKBruteForce(PointCloud& cloud, AABB& boxA, AABB& boxB, RigidBody& bodyA, RigidBody& bodyB) {	
        list<titan::vec3> cornersA = bodyA.GetTransform() * boxA.GetCorners();
        list<titan::vec3> cornersB = bodyB.GetTransform() * boxB.GetCorners();

        for (titan::vec3& cA : cornersA) {
            for (titan::vec3& cB : cornersB) {
                titan::vec3 point = cA - cB;
                cloud.Add(point);
            }
        }

        return false;
    }
    
} // namespace Skynet