#include "joint.h"

Joint::Joint(const uint32_t &index, const JType &type, float* range, float* w) {
    std::copy(w, w + 3, this->w);
    std::copy(range, range + 2, this->range);
    this->index = index;
    this->type = type;
}

Joint::~Joint() {}


float* Joint::getJointTransform() {
    math::clamp(this->a, range);
    switch(type) {
        case(JType::Hinge) : math::axis2T(T, this->w, this->a); return T;
        case(JType::Slide) : math::translate(T, this->w, this->a); return T;
    }
    ASSERT(false, "Unknown joint type");
}

void Joint::updateWorldTransform(float* T, float* pos_w) {   
    float vec[3] = {0.0f, 0.0f, 0.0f};
    math::rotVecQuat(this->p_w, vec, this->q);
    math::addVec3(this->p_w, this->p, pos_w);     // anchor
    math::rotMatVec3(this->w_w, T, this->w);      // axis
}