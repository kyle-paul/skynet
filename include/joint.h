#ifndef SKYNET_JOINT_H
#define SKYNET_JOINT_H

#include "transformable.h"

class Joint : public Transformable
{
public:
    Joint(float w[3]);
    ~Joint();
};

#endif // SKYNET_JOINT_H