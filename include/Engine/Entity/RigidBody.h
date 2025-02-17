#ifndef SKYNET_RIGID_BODY_H
#define SKYNET_RIGID_BODY_H

#include "Math.h"
#include "Timestep.h"
#include "titan.hpp"

namespace Skynet
{  
    class RigidBody
    {
    public:
        RigidBody();
        ~RigidBody();

        inline void SetMass(const float mass) { this->mass = mass; }
        void SetInertialTensor(int x, int y, int z);

        inline titan::mat4 GetTransform() {
            return titan::Transcale(x, s) * titan::Euler2T(omega);
        }

        inline titan::vec3& GetLinearVelocity() { return vel; }
        inline titan::vec3& GetAngularVelocity() { return omega; }

    public:

        /* Constant quantities */ 
        titan::real mass = 0.0f;
        titan::mat3 Ibody;
        titan::mat3 Ibodyinv;

        /* State variables */ 
        titan::vec3 x;
        titan::mat3 R;
        titan::vec3 P;
        titan::vec3 L;

        /* Visualize component */
        titan::vec3 s = {1.0f, 1.0f, 1.0f};
        titan::mat4 T;

        /* Derived quantities */
        titan::mat3 Iinv;
        titan::vec3 vel;
        titan::vec3 omega;

        /* Computed quantities */ 
        titan::vec3 force_ext;
        titan::vec3 force_int;
        titan::vec3 force;
        titan::vec3 torque;
    };

} // namespace Skynet


#endif // SKYNET_RIGID_BODY_H