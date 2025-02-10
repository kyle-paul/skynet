#include "RigidBody.h"
#include "Math.h"

namespace Skynet
{

    RigidBody::RigidBody()
    {

    }

    RigidBody::~RigidBody()
    {

    }

    void RigidBody::UpdateInertial(float x, float y, float z)
    {
        I[0] = (1.0f / 12.0f) * mass * (y * y + z * z);
        I[4] = (1.0f / 12.0f) * mass * (x * x + z * z);
        I[8] = (1.0f / 12.0f) * mass * (x * x + y * y);
    }

    void RigidBody::AddForceAtPos(float* com, float* pos, float* force)
    {
        float offset[3], delta_torque[3];

        /* Add linear force */
        Math::AddVec3(this->force, this->force, force);

        /* Add angular force or torque */ 
        Math::SubVec3(offset, pos, com);
        Math::CrossVec3(delta_torque, force, offset);
        Math::AddVec3(this->torque, this->torque, delta_torque);
    }

    void RigidBody::Update(Timestep* timestep, float* pos, float* euler)
    {
        float dt = timestep->GetSeconds();
        float delta_acc[3], delta_vel[3], delta_pos[3];

        /* Linear motion */

        // accumulate a
        Math::DivVecS3(delta_acc, this->force, mass);
        Math::AddVec3(this->acc, this->acc, delta_acc);

        // v' = v + a*dt
        Math::AddScl3(this->vel, this->acc, dt);

        //p' = p + v*dt;
        Math::AddScl3(pos, this->vel, dt);
        

        /* Angular motion */
        float delta_alpha[3];

        // delta_a =  I_inv * tau
        std::copy(I, I + 9, I_inv);
        
        I_inv[0] = 1.0f / I[0];
        I_inv[4] = 1.0f / I[4];
        I_inv[8] = 1.0f / I[8];

        Math::MulMatVec3R(delta_alpha, I_inv, this->torque);
        Math::AddScl3(this->alpha, delta_alpha, dt);

        // w' = w + a*dt
        Math::AddScl3(this->omega, this->alpha, dt * 10);
        std::copy(this->omega, this->omega + 3, euler);

        // Reset
        Math::Zero3(this->acc);
        Math::Zero3(this->vel);
        Math::Zero3(this->alpha);
        Math::Zero3(this->force);
        Math::Zero3(this->torque);
    }

} // namespace Skynet