#ifndef SKYNET_JOINT_H
#define SKYNET_JOINT_H

#include "data.h"
#include "math.h"
#include "transformable.h"

class Joint : public Transformable
{
public:
    Joint(const uint32_t &index, const JType &type, float* range, float* w);
    ~Joint();

    float* getJointTransform();
    void updateWorldTransform(float* T, float* pos_w);
    
public:
    uint32_t index = 0;
    float delta_a  = 0.0f;
    float range[2] = {0.0f, 0.0f};

    float w_w[3]  = {0.0f, 0.0f, 0.0f};
    float p_w[3]  = {0.0f, 0.0f, 0.0f};
    float cdof[6] = {0.0f, 0.0f, 0.0f};
    
    JType type;
};

#endif // SKYNET_JOINT_H