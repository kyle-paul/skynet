#include "math.h"
#include <iostream>
#include <iomanip>

namespace math {

void quat2T(float* T, float* q, float* p) {
    float x = q[1], y = q[2], z = q[3], w = q[0];
    float x2 = x + x, y2 = y + y, z2 = z + z;
    float xx = x * x2, xy = x * y2, xz = x * z2;
    float yy = y * y2, yz = y * z2, zz = z * z2;
    float wx = w * x2, wy = w * y2, wz = w * z2;

    T[0] = 1.0f - (yy + zz);  T[4] = xy - wz;          T[8]  = xz + wy;          T[12] = p[0];
    T[1] = xy + wz;           T[5] = 1.0f - (xx + zz); T[9]  = yz - wx;          T[13] = p[1];
    T[2] = xz - wy;           T[6] = yz + wx;          T[10] = 1.0f - (xx + yy); T[14] = p[2];
    T[3] = 0.0f;              T[7] = 0.0f;             T[11] = 0.0f;             T[15] = 1.0f;
}

void quat2R(float* R, float* q) {
    normVec4(q);
    float x = q[1], y = q[2], z = q[3], w = q[0];
    float x2 = x + x, y2 = y + y, z2 = z + z;
    float xx = x * x2, xy = x * y2, xz = x * z2;
    float yy = y * y2, yz = y * z2, zz = z * z2;
    float wx = w * x2, wy = w * y2, wz = w * z2;

    R[0] = 1.0f - (yy + zz);  R[4] = xy - wz;           R[8] = xz + wy;            R[12] = 0.0f;
    R[1] = xy + wz;           R[5] = 1.0f - (xx + zz);  R[9] = yz - wx;            R[13] = 0.0f;
    R[2] = xz - wy;           R[6] = yz + wx;           R[10] = 1.0f - (xx + yy);  R[14] = 0.0f;
    R[3] = 0.0f;              R[7] = 0.0f;              R[11] = 0.0f;              R[15] = 1.0f;
}

void euler2V(float* T, float* e, float *p) {
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

void axis2R(float* R, float* w, float theta) {
    const float cs_ = cos(DEG2RAD(theta));
    const float sn_ = sin(DEG2RAD(theta));
    normVec3(w); float t[3];

    t[0] = (1 - cs_) * w[0];
    t[1] = (1 - cs_) * w[1];
    t[2] = (1 - cs_) * w[2];

    R[0] = cs_ + t[0] * w[0];          R[4] = t[0] * w[1] + sn_ * w[2];   R[8]  = t[0] * w[2] - sn_ * w[1];   R[12] = 0.0f;
    R[1] = t[0] * w[1] - sn_ * w[2];   R[5] = cs_ + t[1] * w[1];          R[9]  = t[1] * w[2] + sn_ * w[0];   R[13] = 0.0f;
    R[2] = t[0] * w[2] + sn_ * w[1];   R[6] = t[1] * w[2] - sn_ * w[0];   R[10] = cs_ + t[2] * w[2];          R[14] = 0.0f;
    R[3] = 0.0f;                       R[7] = 0.0f;                       R[11] = 0.0f;                       R[15] = 1.0f;
}

void perspective(float* P, float& fov, float& aspect, float& znear, float &zfar) {
    float tanHalfFov = tan(DEG2RAD(fov) / 2.0f);
    float zrange = znear - zfar;

    P[0] = 1 / (aspect * tanHalfFov);  P[4] = 0.0f;             P[8]  = 0.0f;                     P[12] = 0.0f;
    P[1] = 0.0f;                       P[5] = 1 / tanHalfFov;   P[9]  = 0.0f;                     P[13] = 0.0f;
    P[2] = 0.0f;                       P[6] = 0.0f;             P[10] = (zfar + znear) / zrange;  P[14] = (2.0f * zfar * znear) / zrange;
    P[3] = 0.0f;                       P[7] = 0.0f;             P[11] = -1.0f;                    P[15] = 0.0f;
}

void matmul(float* res, float* m1, float* m2, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            res[i + j * n] = 0.0f;
            for (int k = 0; k < n; k++) {
                res[i + j * n] += m1[i + k * n] * m2[k + j * n];
            }
        }
    }
}

void matmul4(float* res, float* m1, float* m2) {
    res[0] = m2[0] * m1[0] + m2[1] * m1[4] + m2[2] * m1[8] + m2[3] * m1[12];
    res[1] = m2[0] * m1[1] + m2[1] * m1[5] + m2[2] * m1[9] + m2[3] * m1[13];
    res[2] = m2[0] * m1[2] + m2[1] * m1[6] + m2[2] * m1[10] + m2[3] * m1[14];
    res[3] = m2[0] * m1[3] + m2[1] * m1[7] + m2[2] * m1[11] + m2[3] * m1[15];

    res[4] = m2[4] * m1[0] + m2[5] * m1[4] + m2[6] * m1[8] + m2[7] * m1[12];
    res[5] = m2[4] * m1[1] + m2[5] * m1[5] + m2[6] * m1[9] + m2[7] * m1[13];
    res[6] = m2[4] * m1[2] + m2[5] * m1[6] + m2[6] * m1[10] + m2[7] * m1[14];
    res[7] = m2[4] * m1[3] + m2[5] * m1[7] + m2[6] * m1[11] + m2[7] * m1[15];

    res[8] = m2[8] * m1[0] + m2[9] * m1[4] + m2[10] * m1[8] + m2[11] * m1[12];
    res[9] = m2[8] * m1[1] + m2[9] * m1[5] + m2[10] * m1[9] + m2[11] * m1[13];
    res[10] = m2[8] * m1[2] + m2[9] * m1[6] + m2[10] * m1[10] + m2[11] * m1[14];
    res[11] = m2[8] * m1[3] + m2[9] * m1[7] + m2[10] * m1[11] + m2[11] * m1[15];

    res[12] = m2[12] * m1[0] + m2[13] * m1[4] + m2[14] * m1[8] + m2[15] * m1[12];
    res[13] = m2[12] * m1[1] + m2[13] * m1[5] + m2[14] * m1[9] + m2[15] * m1[13];
    res[14] = m2[12] * m1[2] + m2[13] * m1[6] + m2[14] * m1[10] + m2[15] * m1[14];
    res[15] = m2[12] * m1[3] + m2[13] * m1[7] + m2[14] * m1[11] + m2[15] * m1[15];
}

void matmul3(float *res, float* m1, float* m2) {
    res[0] = m2[0] * m1[0] + m2[1] * m1[3] + m2[2] * m1[6];
    res[1] = m2[0] * m1[1] + m2[1] * m1[4] + m2[2] * m1[7];
    res[2] = m2[0] * m1[2] + m2[1] * m1[5] + m2[2] * m1[8];

    res[3] = m2[3] * m1[0] + m2[4] * m1[3] + m2[5] * m1[6];
    res[4] = m2[3] * m1[1] + m2[4] * m1[4] + m2[5] * m1[7];
    res[5] = m2[3] * m1[2] + m2[4] * m1[5] + m2[5] * m1[8];

    res[6] = m2[6] * m1[0] + m2[7] * m1[3] + m2[8] * m1[6];
    res[7] = m2[6] * m1[1] + m2[7] * m1[4] + m2[8] * m1[7];
    res[8] = m2[6] * m1[2] + m2[7] * m1[5] + m2[8] * m1[8];
}

void invert4(float* res, float* m) {
    res[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
    res[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];
    res[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];
    res[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];
    res[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
    res[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
    res[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];
    res[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];
    res[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];
    res[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];
    res[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];
    res[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];
    res[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];
    res[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];
    res[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];
    res[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];

    float det = m[0] * res[0] + m[1] * res[4] + m[2] * res[8] + m[3] * res[12];
    det = 1.0f / det;
    for (uint32_t i = 0; i < 16; i++) res[i] = res[i] * det;
}

ImVec4 mulmatvec4(float* m, const ImVec4& v) {
    const float x = m[0] * v.x + m[4] * v.y + m[8] * v.z + m[12] * v.w;
    const float y = m[1] * v.x + m[5] * v.y + m[9] * v.z + m[13] * v.w;
    const float z = m[2] * v.x + m[6] * v.y + m[10] * v.z + m[14] * v.w;
    const float w = m[3] * v.x + m[7] * v.y + m[11] * v.z + m[15] * v.w;
    return { x, y, z, w };
}

void normVec3(float* v) {
    float norm = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    if (norm < MINVAL) {
        v[0] = 1; v[1] = 0; v[2] = 0;
    } else {
        float normInv = 1/norm;
        v[0] *= normInv;
        v[1] *= normInv;
        v[2] *= normInv;
    }
}

void normVec4(float* v) {
    float norm = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2] + v[3]*v[3]);
    if (norm < MINVAL) {
        v[0] = 1; v[1] = 0;
        v[2] = 0; v[3] = 0;
    } else {
        float normInv = 1/norm;
        v[0] *= normInv;
        v[1] *= normInv;
        v[2] *= normInv;
        v[3] *= normInv;
    }
}

void printMat4(float* m) {
    std::cout << std::fixed << std::setprecision(2);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << m[j * 4 + i] << ' ';
        } std::cout << '\n';
    } std::cout << '\n';
}

void printMat3(float* m) {
    std::cout << std::fixed << std::setprecision(2);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            std::cout << m[j * 3 + i] << ' ';
        } std::cout << '\n';
    } std::cout << '\n';
}

void printVec4(float* v) {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << v[0] << ' ' << v[1] << ' ' << v[2] << ' ' << v[3] << '\n';
}

void printVec3(float* v) {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << v[0] << ' ' << v[1] << ' ' << v[2] << '\n';
}

void transpose(float* m) {
    float temp;
    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 4; j++) {
            temp = m[i + j * 4];
            m[i + j * 4] = m[j + i * 4];
            m[j + i * 4] = temp;
        }
    }
}

const float* identity() {
    static float m[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    }; return m;
}

void vizgraph(std::unordered_map<std::string, std::vector<std::string>> &graph) {
    for (auto &[name, vec] : graph) {
        std::cout << name << ": {";
        for (auto &v : graph[name]) {
            std::cout << v << ' ';
        } std::cout << "} \n";
    }
}

}