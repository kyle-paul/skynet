#include "math.h"
#include <iostream>
#include <iomanip>

namespace math {

void printMat4(float* mat) {
    std::cout << std::fixed << std::setprecision(2);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << mat[j * 4 + i] << ' ';
        } std::cout << '\n';
    } std::cout << '\n';
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


}