#include "math.h"
#include <iostream>
#include <iomanip>

namespace math {

void printMat4(float* mat) {
    std::cout << std::fixed << std::setprecision(2);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << mat[i * 4 + j] << ' ';
        } std::cout << '\n';
    } std::cout << '\n';
}

void transpose(float* mat) {
    float temp;
    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 4; j++) {
            temp = mat[i + j * 4];
            mat[i + j * 4] = mat[j + i * 4];
            mat[j + i * 4] = temp;
        }
    }
}

const float* identity() {
    static float mat[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    }; return mat;
}

void quat2TF(float* T, float* q, float* p) {
    float x = q[0], y = q[1], z = q[2], w = q[3];
    float x2 = x + x, y2 = y + y, z2 = z + z;
    float xx = x * x2, xy = x * y2, xz = x * z2;
    float yy = y * y2, yz = y * z2, zz = z * z2;
    float wx = w * x2, wy = w * y2, wz = w * z2;

    T[0] = 1.0f - (yy + zz);  T[4] = xy - wz;          T[8]  = xz + wy;          T[12] = p[0];
    T[1] = xy + wz;           T[5] = 1.0f - (xx + zz); T[9]  = yz - wx;          T[13] = p[1];
    T[2] = xz - wy;           T[6] = yz + wx;          T[10] = 1.0f - (xx + yy); T[14] = p[2];
    T[3] = 0.0f;              T[7] = 0.0f;             T[11] = 0.0f;             T[15] = 1.0f;
}

void euler2TF(float* T, float* e, float *p) {
	float cx = cos(e[0]);
    float cy = cos(e[1]);
    float cz = cos(e[2]);
    float sx = sin(e[0]);
    float sy = sin(e[1]);
    float sz = sin(e[2]);
	
 	T[0] = cz*cy;  T[4] = cz*sy*sx - sz*cx;  T[8]  = cz*sy*cx + sz*sx;  T[12] = -p[0];
    T[1] = sz*cy;  T[5] = sz*sy*sx + cz*cx;  T[9]  = sz*sy*cx - cz*sx;  T[13] = -p[1];
    T[2] = -sy;    T[6] = cy*sx;             T[10] = cy*cx;             T[14] = -p[2];
    T[3] = 0.0f;   T[7] = 0.0f;              T[11] = 0.0f,              T[15] =  1.0f;
}

void perspective(float* P, float& fov, float& aspect, float& znear, float &zfar) {
    float tanHalfFov = tan(DEG2RAD(fov) / 2.0f);
    float zrange = znear - zfar;

    P[0] = 1 / (aspect * tanHalfFov);  P[4] = 0.0f;             P[8]  = 0.0f;                     P[12] = 0.0f;
    P[1] = 0.0f;                       P[5] = 1 / tanHalfFov;   P[9]  = 0.0f;                     P[13] = 0.0f;
    P[2] = 0.0f;                       P[6] = 0.0f;             P[10] = (zfar + znear) / zrange;  P[14] = (2.0f * zfar * znear) / zrange;
    P[3] = 0.0f;                       P[7] = 0.0f;             P[11] = -1.0f;                    P[15] = 0.0f;
}

void matmul4(float* res, float* mat1, float* mat2) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            res[i + j * 4] = 0.0f;
            for (int k = 0; k < 4; k++) {
                res[i + j * 4] += mat1[i + k * 4] * mat2[k + j * 4];
            }
        }
    }
}

void matmul3(float* res, float* mat1, float *mat2) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            res[i + j * 3] = 0.0f;
            for (int k = 0; k < 3; k++) {
                res[i + j * 3] += mat1[i + k * 3] * mat2[k + j * 3];
            }
        }
    }
}

}