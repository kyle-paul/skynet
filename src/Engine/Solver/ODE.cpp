#include "ODE.h"
#include "Data.h"
#include "Math.h"
#include "Component.h"
#include "Titan.hpp"

namespace Skynet
{
    void ODE::InitializeStateArray(RigidBody* body, float* y)
    {
        /* position component */ 
        *y++ = body->x[0];
        *y++ = body->x[1];
        *y++ = body->x[2];

        /* Rotation component */ 
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                *y++ = body->R(i,j);

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
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                body->R(i,j) = *y++;

        /* Linear momentum component */
        body->P[0] = *y++;
        body->P[1] = *y++;
        body->P[2] = *y++;

        /* Angular momentum component */ 
        body->L[0] = *y++;
        body->L[1] = *y++;
        body->L[2] = *y++;

        /* Compute auxiliary variables */
        body->vel = body->P * (1 / body->mass);

        /* Iinv = R * Ibody * R_T */ 
        body->Iinv = body->R * body->Ibody * body->R.transpose();

        /* omega = Iinv * L */ 
        body->omega = body->Iinv * body->L;
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
        /* External force */
        body->force_ext.zero();
        body->torque.zero();

        body->force_ext[1] -= body->mass * 9.81f;

        /* Accumulate all force */ 
        body->force = body->force_int + body->force_ext;
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
        titan::mat3 Rdot = titan::Skew(body->omega) * body->R;
        
        for (int i = 0; i < 3; i++) 
            for (int j = 0; j < 3; j++)
                *ydot++ = Rdot(i,j);

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
            y[i] += dt * ydot[i];
        }

        ArrayToBodies(bodies, y);
    }

    void ODE::Midpoint(entt::registry& bodies, float t, float dt, float* y, float* ydot)
    {
        BodiesToArray(bodies, y);

        float y_mid[STATE_SIZE];

        /* Step 1: Compute the midpoint */
        Derivative(bodies, t, y, ydot);
        for (int i = 0; i < STATE_SIZE; ++i) {
            y_mid[i] = y[i] + 0.5f * dt * ydot[i];
        }

        /* Step 2: Compute final */
        Derivative(bodies, t + 0.5f * dt, y_mid, ydot);
        for (int i = 0; i < STATE_SIZE; ++i) {
            y[i] = y[i] + dt * ydot[i];
        }

        ArrayToBodies(bodies, y);
    }

    void ODE::RungeKutta(entt::registry& bodies, float t, float dt, float* y, float* ydot)
    {
        BodiesToArray(bodies, y);

        float k1[STATE_SIZE], k2[STATE_SIZE], k3[STATE_SIZE], k4[STATE_SIZE];
        float y_temp[STATE_SIZE];

        /* Step 1: Compute k1 */
        Derivative(bodies, t, y, k1);
        for (int i = 0; i < STATE_SIZE; ++i) {
            y_temp[i] = y[i] + (dt * 0.5f) * k1[i];
        }

        /* Step 2: Compute k2 */
        Derivative(bodies, t + dt * 0.5f, y_temp, k2);
        for (int i = 0; i < STATE_SIZE; ++i) {
            y_temp[i] = y[i] + (dt * 0.5f) * k2[i];
        }

        /* Step 3: Compute k3 */
        Derivative(bodies, t + dt * 0.5f, y_temp, k3);
        for (int i = 0; i < STATE_SIZE; ++i) {
            y_temp[i] = y[i] + dt * k3[i];
        }

        /* Step 4: Compute k4 */
        Derivative(bodies, t + dt, y_temp, k4);
        for (int i = 0; i < STATE_SIZE; ++i) {
            y[i] = y[i] + (dt / 6) * (k1[i] + 2 * k2[i] + 2 * k3[i] + k4[i]);
        }

        ArrayToBodies(bodies, y);
    }

} // namespace Skynet