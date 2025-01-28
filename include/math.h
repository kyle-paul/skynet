#pragma once
#include "core.h"
#include <cmath>

#define PI 3.14159265358979323846
#define RAD2DEG(rad) ((rad) * (180.0 / PI))
#define DEG2RAD(deg) ((deg) * (PI / 180.0))
#define MINVAL 1E-15f

namespace math {

void quat2T(float* T, float* q, float* p);
void quat2R(float* R, float* q);
void euler2V(float* T, float* e, float* p);
void axis2R(float* R, float* w, float theta);
void perspective(float* P, float& fov, float& aspect, float& znear, float &zfar);

void matmul4(float* res, float* m1, float* m2);
void matmul3(float* res, float* m1, float* m2);
void normVec3(float* v);
void normVec4(float* v);

void printMat4(float* m);
void printMat3(float* m);
void printVec4(float* v);
void printVec3(float* v);

void transpose(float* m);
const float* identity();

void vizgraph(std::unordered_map<std::string, std::vector<std::string>> &graph);

} // namespace math