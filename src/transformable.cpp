#include "transformable.h"

void Transformable::updateRotatation(const Rot &type)
{
    const float cs_ = cos(DEG2RAD(theta));
    const float sn_ = sin(DEG2RAD(theta));

    theta += 0.9;
    switch(type) {
        case(Rot::Xaxis) : {
            R[0] = 1.0f;  R[0] = 0.0f;  R[0] = 0.0f;   R[0] = 0.0f;
            R[1] = 0.0f;  R[1] = cs_;   R[1] = -sn_;   R[1] = 0.0f;
            R[2] = 0.0f;  R[2] = sn_;   R[2] = cs_;    R[2] = 0.0f;
            R[3] = 0.0f;  R[3] = 0.0f;  R[3] = 0.0f;   R[3] = 1.0f;
            return;
        }
        case(Rot::Yaxis) : {
            R[0] = cs_;   R[4] = 0.0f;  R[8]  = -sn_;  R[12] = 0.0f;
            R[1] = 0.0f;  R[5] = 1.0f;  R[9]  = 0.0f;  R[13] = 0.0f;
            R[2] = sn_;   R[6] = 0.0f;  R[10] = cs_;   R[14] = 0.0f;
            R[3] = 0.0f;  R[7] = 0.0f;  R[11] = 0.0f;  R[15] = 1.0f;
            return;
        }
        case(Rot::Zaxis) : {
            R[0] = cs_;   R[4] = -sn_;  R[8]  = 0.0f;  R[12] = 0.0f;
            R[1] = sn_;   R[5] = cs_;   R[9]  = 0.0f;  R[13] = 0.0f;
            R[2] = 0.0f;  R[6] = 0.0f;  R[10] = 1.0f;  R[14] = 0.0f;
            R[3] = 0.0f;  R[7] = 0.0f;  R[11] = 0.0f;  R[15] = 1.0f;
            return;
        }
        case(Rot::Waxis) : {
        	float t[3];

			t[0] = (1 - cs_) * w[0];
			t[1] = (1 - cs_) * w[1];
			t[2] = (1 - cs_) * w[2];

            R[0] = cs_ + t[0] * w[0];          R[4] = t[0] * w[1] + sn_ * w[2];   R[8]  = t[0] * w[2] - sn_ * w[1];   R[12] = 0.0f;
            R[1] = t[0] * w[1] - sn_ * w[2];   R[5] = cs_ + t[1] * w[1];          R[9]  = t[1] * w[2] + sn_ * w[0];   R[13] = 0.0f;
            R[2] = t[0] * w[2] + sn_ * w[1];   R[6] = t[1] * w[2] - sn_ * w[0];   R[10] = cs_ + t[2] * w[2];          R[14] = 0.0f;
            R[3] = 0.0f;                       R[7] = 0.0f;                       R[11] = 0.0f;                       R[15] = 1.0f;
            return;
        }
    }
    ASSERT(false, "Must choose rotation type");
}

void Transformable::updateTransform() {
    float T_[16];   
	T_[0] = s[0];  T_[4] = 0.0f;  T_[8] = 0.0f;   T_[12] = p[0];
    T_[1] = 0.0f;  T_[5] = s[1];  T_[9] = 0.0f;   T_[13] = p[1];
    T_[2] = 0.0f;  T_[6] = 0.0f;  T_[10] = s[2];  T_[14] = p[2];
    T_[3] = 0.0f;  T_[7] = 0.0f;  T_[11] = 0.0f;  T_[15] = 1.0f;

    math::quat2R(R, q);
    math::matmul4(T, T_, R);
}