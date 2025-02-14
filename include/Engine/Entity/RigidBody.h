#ifndef SKYNET_RIGID_BODY_H
#define SKYNET_RIGID_BODY_H

#include "Math.h"
#include "Timestep.h"

namespace Skynet
{  
    class RigidBody
    {
    public:
        RigidBody();
        ~RigidBody();

        inline void SetMass(const float mass) { this->mass = mass; }
        void SetInertialTensor(int x, int y, int z);

        float* GetTransform();

        inline float* GetLinearVelocity() { return vel; }
        inline float* GetAngularVelocity() { return omega; }

    public:

        /* Constant quantities */ 
        float mass        = 0.0f;
        float Ibody[9]    = {0.0f};
        float Ibodyinv[9] = {0.0f};

        /* State variables */ 
        float x[3] = {0.0f};
        float R[9] = {0.0f};
        float P[3] = {0.0f};
        float L[3] = {0.0f};

        /* Visualize component */
        float s[3] = {1.0f, 1.0f, 1.0f};
        float T[9] = {0.0f};

        /* Derived quantities */
        float Iinv[9]  = {0.0f};
        float vel[3]   = {0.0f};
        float omega[3] = {0.0f};

        /* Computed quantities */ 
        float force_ext[3] = {0.0f};
        float force_int[3] = {0.0f};
        float force[3]  = {0.0f};
        float torque[3] = {0.0f};
    };

} // namespace Skynet


#endif // SKYNET_RIGID_BODY_H