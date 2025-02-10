#ifndef SKYNET_RIGID_BODY_H
#define SKYNET_RIGID_BODY_H

#include "Timestep.h"

namespace Skynet
{
    /*
    This class provide different functions for computing
    physics properties of a rigid body, interface for
    different mesh type.
    */

    class RigidBody
    {
    public:
        RigidBody();
        ~RigidBody();

        void UpdateInertial(float x, float y, float z);
        void AddForceAtPos(float* com, float* pos, float* force);
        void Update(Timestep* timestep, float* com, float* euler);

        inline void SetMass(const float mass) { this->mass = mass; }

        inline float* GetLinearVelocity() { return vel; }
        inline float* GetAngularVelocity() { return omega; }

    private:

        float mass = 0.0f;

        float I[9] = {0.0f};
        float I_inv[9] = {0.0f};
        float I_body[9] = {0.0f};
        float I_body_inv[9] = {0.0f};

        float vel[3] = {0.0f};
        float acc[3] = {0.0f};

        float omega[3] = {0.0f};
        float alpha[3] = {0.0f};

        float force[3] = {0.0f};
        float torque[3] = {0.0f};

        float force_cons[3] = {0.0f};
        float torque_cons[3] = {0.0f};

        friend class Serializer;
    };;

} // namespace Skynet


#endif // SKYNET_RIGID_BODY_H