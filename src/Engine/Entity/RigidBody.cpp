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

    void RigidBody::SetInertialTensor(int x, int y, int z)
    {
        Ibody[0] = this->mass / 12 * (y*y +  z*z);
        Ibody[4] = this->mass / 12 * (x*x +  z*z);
        Ibody[8] = this->mass / 12 * (x*x +  y*y);
    }

    float* RigidBody::GetTransform() 
    {
        float T_[16]; float R_[16];
        Math::Transcale(T_, x, s);
        Math::Euler2T(R_, omega);
        Math::Matmul4(T, T_, R_);
        return T;
    }
    
} // namespace Skynet