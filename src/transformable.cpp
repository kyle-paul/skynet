#include "transformable.h"

void Transformable::updateRotatation(const RotAxis &type)
{
    switch(type) {
        case(RotAxis::Xaxis) : math::axisX2T(R, w, a); return;
        case(RotAxis::Yaxis) : math::axisY2T(R, w, a); return;
        case(RotAxis::Zaxis) : math::axisZ2T(R, w, a); return;
        case(RotAxis::Waxis) : math::axis2T (R, w, a); return;
    }
    ASSERT(false, "Must choose rotation type");
}

void Transformable::updateTransform(const RotType &type) {
    float T_[16]; math::transcale(T_, p, s);
    switch(type) {
        case(RotType::Quaternion) : math::quat2T(R, q);  break;
        case(RotType::Euler)      : math::euler2T(R, e); break;
    }
    math::matmul4(T, T_, R);
}