#pragma once
#include <cmath>

#define PI 3.14159265358979323846
#define RAD2DEG(rad) ((rad) * (180.0 / PI))
#define DEG2RAD(deg) ((deg) * (PI / 180.0))


namespace math {

void euler2TF(float* res, float* e, float* p);
void perspective(float* P, float& fov, float& aspect, float& znear, float &zfar);

}
