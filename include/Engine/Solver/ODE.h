#ifndef SKYNET_SOLVER_ODE_H
#define SKYNET_SOLVER_ODE_H

#include "entt.hpp"
#include "System.h"
#include "RigidBody.h"

namespace Skynet
{
    class ODE
    {
    public:
        static void ArrayToBodies(entt::registry& bodies, float* y);
        static void BodiesToArray(entt::registry& bodies, float* y);

        static void InitializeStateArray(RigidBody* body, float* y);
        static void DecomposeStateArray(RigidBody* body, float* y);

        static void ComputeForceTorque(float t, RigidBody* body);
        static void Derivative(entt::registry& bodies, float t, float* y, float* ydot);
        static void DydtStateArray(RigidBody* body, float* ydot);

        static void EulerStep(entt::registry& bodies, float t, float dt, float* y, float* ydot);
    };

} // namespace Skynet


#endif // SKYNET_SOLVER_ODE_H