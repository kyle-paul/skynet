#ifndef SKYNET_MATH_H
#define SKYNET_MATH_H

#include "core.h"
#include "imgui.h"

#define PI 3.14159265358979323846
#define RAD2DEG(rad) ((rad) * (180.0 / PI))
#define DEG2RAD(deg) ((deg) * (PI / 180.0))
#define MINVAL 1E-15f

namespace math {

void quat2T  (float* T, float* q, float* p = nullptr);
void euler2T (float* T, float* e, float* p = nullptr);
void axisX2T (float* T, float* w, float &a, float* p = nullptr);
void axisY2T (float* T, float* w, float &a, float* p = nullptr);
void axisZ2T (float* T, float* w, float &a, float* p = nullptr);
void axis2T  (float* T, float* w, float &a, float* p = nullptr);
void perspective(float* P, float& fov, float& aspect, float& znear, float &zfar);
void transcale (float* T, float* p, float* s);
void translate (float* T, float* w, float &a);
void decompose(float* T, float* p, float* s, float* e);
void decomposeTw(float* T, float* p, float* q);

void matmul(float* res, float* m1, float* m2, int m, int n, int q);
void matmulsq(float* res, float* m1, float* m2, int n);
void matmul3(float* res, float* m1, float* m2);
void matmul4(float* res, float* m1, float* m2);
void invert3(float* res, float* m);
void invert4(float* res, float* m);
void transpose(float* mat_T, float* mat, int n, int m);
void transpose3(float* m);
void transpose4(float* m);
void identity3(float* m);
void identity4(float* m);

void mulMatVec(float* res, float* mat, float* vec, int n, int m);
void mulMatVec4(float* res, float* mat, float* vec);
void rotMatVec3(float* res, float* mat, float* vec);

void addVec3(float* res, float* v1, float* v2);
void addVecS3(float* res, float* v1, float &a);
void subVec3(float* res, float* v1, float* v2);
void mulVecS3(float* res, float* v, float &s);
void divVecS3(float* res, float* v, float &s);
float normVec3(float* v);
float normVec4(float* v);
float normVec(float* v, int n);
void cross3(float* res, float* v1, float* v2);

void clamp(float &a, float* range);
void invert(float* m, int n);

ImVec4 mulmatvec4(float* m, const ImVec4& v);
void rotVecQuat(float res[3], const float vec[3], const float quat[4]);
void copy3(float* res, const float* v);
void zero3(float* v);

void printMat(float* mat, int n, int m);
void printMat3(float* m);
void printMat4(float* m);
void printVec3(float* v);
void printVec4(float* v);
void printVec(float* v, int n);

void vizgraph(std::unordered_map<std::string, std::vector<std::string>> &graph);

} // namespace math

#endif // SKYNET_MATH_H