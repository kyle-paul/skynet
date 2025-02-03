#include "transformable.h"

float* Transformable::getLocalTransform(const RotType &type) {
    float T_[16]; math::transcale(T_, p, s);
    switch(type) {
        case(RotType::Quaternion) : math::quat2T(R, q);  break;
        case(RotType::Euler)      : math::euler2T(R, e); break;
    }
    math::matmul4(T, T_, R);
    return T;
}