#include "RigidBody.h"
#include "Math.h"
#include "titan.hpp"

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
        Ibody(0,0) = this->mass / 12 * (y*y +  z*z);
        Ibody(1,1) = this->mass / 12 * (x*x +  z*z);
        Ibody(2,2) = this->mass / 12 * (x*x +  y*y);
    }
    
} // namespace Skynet