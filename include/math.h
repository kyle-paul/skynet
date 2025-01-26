#pragma once
#include <cmath>

#define PI 3.14159265358979323846
#define RAD2DEG(rad) ((rad) * (180.0 / PI))
#define DEG2RAD(deg) ((deg) * (PI / 180.0))


namespace math {

void euler2TF(float* T, float* e, float* p);
void perspective(float* P, float& fov, float& aspect, float& znear, float &zfar);
void matmul4(float* res, float* mat1, float* mat2);
void printMat4(float* mat);
void transpose(float* mat);
const float* identity();

} // namespace math