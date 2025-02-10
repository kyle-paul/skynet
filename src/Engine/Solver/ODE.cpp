#include "ODE.h"
#include "Data.h"
#include "Math.h"
#include "Component.h"

namespace Skynet
{
    void ODE::InitializeStateArray(RigidBody* body, float* y)
    {
        /* position component */ 
        *y++ = body->x[0];
        *y++ = body->x[1];
        *y++ = body->x[2];

        /* Rotation component */ 
        for (int i=0; i<9; i++) 
            *y++ = body->R[i];

        /* Linear momentum component */
        *y++ = body->P[0];
        *y++ = body->P[1];
        *y++ = body->P[2];

        /* Angular momentum component */ 
        *y++ = body->L[0];
        *y++ = body->L[1];
        *y++ = body->L[2];
    }

    void ODE::DecomposeStateArray(RigidBody* body, float* y)
    {
        body->x[0] = *y++;
        body->x[1] = *y++;
        body->x[2] = *y++;

        /* Rotation component */ 
        for (int i=0; i<9; i++) 
            body->R[i] = *y++; 

        /* Linear momentum component */
        body->P[0] = *y++;
        body->P[1] = *y++;
        body->P[2] = *y++;

        /* Angular momentum component */ 
        body->L[0] = *y++;
        body->L[1] = *y++;
        body->L[2] = *y++;

        /* Compute auxiliary variables */
        Math::DivVecS3(body->vel, body->P, body->mass);

        float R_transpose[9], temp[9];
        Math::Transpose(R_transpose, body->R, 3, 3);

        /* Iinv = R * Ibody * R_T */ 
        Math::Matmul(temp, body->R, body->Ibody, 3, 3, 3);
        Math::Matmul(body->Iinv, temp, R_transpose, 3, 3, 3);

        /* omega = Iinv * L */ 
        Math::Matmul(body->omega, body->Iinv, body->L, 3, 3, 1);
    }   

    void ODE::BodiesToArray(entt::registry& bodies, float* y)
    {
        int i = 0;
        auto view = bodies.view<RigidBodyComp>();
        for (auto entity : view)
        {
            auto& rb = view.get<RigidBodyComp>(entity);
            if (rb.type == BodyType::Dynamic)
                InitializeStateArray(&rb.body, y + STATE_SIZE * i++);
        }
    }

    void ODE::ArrayToBodies(entt::registry& bodies, float* y)
    {
        int i = 0;
        auto view = bodies.view<RigidBodyComp>();
        for (auto entity : view)
        {
            auto& rb = view.get<RigidBodyComp>(entity);
            if (rb.type == BodyType::Dynamic)
                DecomposeStateArray(&rb.body, y + STATE_SIZE * i++);
        }
    }

    void ODE::ComputeForceTorque(float t, RigidBody* body)
    {
        Math::Zero3(body->force);
        Math::Zero3(body->torque);

        body->force[1] -= body->mass * 0.81f;
    }

    void ODE::Derivative(entt::registry& bodies, float t, float* y, float* ydot)
    {
        auto view = bodies.view<RigidBodyComp>();
        for (auto entity : view)
        {
            auto& rb = view.get<RigidBodyComp>(entity); 
            if (rb.type == BodyType::Dynamic) 
            {
                ComputeForceTorque(t, &rb.body);
                DydtStateArray(&rb.body, ydot);
            }   
        }
    }

    void ODE::DydtStateArray(RigidBody* body, float* ydot)
    {
        /* Compute linear velocity */ 
        *ydot++ = body->vel[0];
        *ydot++ = body->vel[1];
        *ydot++ = body->vel[2];

        /* Compute angular velocity */ 
        float Rdot[9], SkewOmega[9];
        Math::SkewMatrix(SkewOmega, body->omega);
        Math::Matmul(Rdot, SkewOmega, body->R, 3, 3, 3);
        for (int i=0; i<9; i++) *ydot++ = Rdot[i];

        /* d/dt P(t) = F(t) */ 
        *ydot++ = body->force[0];
        *ydot++ = body->force[1];
        *ydot++ = body->force[2];

        /* d/dt L(t) = T(t) */ 
        *ydot++ = body->torque[0];
        *ydot++ = body->torque[1];
        *ydot++ = body->torque[2];
    }

    void ODE::EulerStep(entt::registry& bodies, float t, float dt, float* y, float* ydot)
    {
        BodiesToArray(bodies, y);
        
        Derivative(bodies, t, y, ydot);

        for (int i = 0; i < STATE_SIZE; ++i) 
        {
            y[i] += ydot[i] * dt;
        }

        ArrayToBodies(bodies, y);
    }

} // namespace Skynet