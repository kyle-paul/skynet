#include "math.h"
#include <iostream>
#include <iomanip>

namespace math {

void quat2T(float* T, float* q, float* p) {
    normVec4(q);
    float x = q[1], y = q[2], z = q[3], w = q[0];
    float x2 = x + x, y2 = y + y, z2 = z + z;
    float xx = x * x2, xy = x * y2, xz = x * z2;
    float yy = y * y2, yz = y * z2, zz = z * z2;
    float wx = w * x2, wy = w * y2, wz = w * z2;

    T[0] = 1.0f - (yy + zz);  T[4] = xy - wz;          T[8]  = xz + wy;          T[12] = p != nullptr ? p[0] : 0.0f;
    T[1] = xy + wz;           T[5] = 1.0f - (xx + zz); T[9]  = yz - wx;          T[13] = p != nullptr ? p[1] : 0.0f;
    T[2] = xz - wy;           T[6] = yz + wx;          T[10] = 1.0f - (xx + yy); T[14] = p != nullptr ? p[2] : 0.0f;
    T[3] = 0.0f;              T[7] = 0.0f;             T[11] = 0.0f;             T[15] = 1.0f;
}

void euler2T(float* T, float* e, float *p) {
	float cx = cos(e[0]);
    float cy = cos(e[1]);
    float cz = cos(e[2]);
    float sx = sin(e[0]);
    float sy = sin(e[1]);
    float sz = sin(e[2]);
	
 	T[0] = cz*cy;  T[4] = cz*sy*sx - sz*cx;  T[8]  = cz*sy*cx + sz*sx;  T[12] = p != nullptr ? -p[0] : 0.0f;
    T[1] = sz*cy;  T[5] = sz*sy*sx + cz*cx;  T[9]  = sz*sy*cx - cz*sx;  T[13] = p != nullptr ? -p[1] : 0.0f;
    T[2] = -sy;    T[6] = cy*sx;             T[10] = cy*cx;             T[14] = p != nullptr ? -p[2] : 0.0f;
    T[3] = 0.0f;   T[7] = 0.0f;              T[11] = 0.0f,              T[15] = 1.0f;
}

void axisX2T (float* T, float* e, float &a, float* p) {
    const float cs = cos(DEG2RAD(a));
    const float sn = sin(DEG2RAD(a));

    T[0] = 1.0f;  T[0] = 0.0f;  T[0] = 0.0f;   T[12] = p != nullptr ? p[0] : 0.0f;
    T[1] = 0.0f;  T[1] = cs;    T[1] = -sn;    T[13] = p != nullptr ? p[1] : 0.0f;
    T[2] = 0.0f;  T[2] = sn;    T[2] = cs;     T[14] = p != nullptr ? p[2] : 0.0f;
    T[3] = 0.0f;  T[3] = 0.0f;  T[3] = 0.0f;   T[3] = 1.0f;
}

void axisY2T (float* T, float* e, float &a, float* p) {
    const float cs = cos(DEG2RAD(a));
    const float sn = sin(DEG2RAD(a));
    
    T[0] = cs;    T[4] = 0.0f;  T[8]  = -sn;   T[12] = p != nullptr ? p[0] : 0.0f;
    T[1] = 0.0f;  T[5] = 1.0f;  T[9]  = 0.0f;  T[13] = p != nullptr ? p[1] : 0.0f;
    T[2] = sn;    T[6] = 0.0f;  T[10] = cs;    T[14] = p != nullptr ? p[2] : 0.0f;
    T[3] = 0.0f;  T[7] = 0.0f;  T[11] = 0.0f;  T[15] = 1.0f;
}

void axisZ2T (float* T, float* e, float &a, float* p) {
    const float cs = cos(DEG2RAD(a));
    const float sn = sin(DEG2RAD(a));
    
    T[0] = cs;    T[4] = -sn;   T[8]  = 0.0f;  T[12] = p != nullptr ? p[0] : 0.0f;
    T[1] = sn;    T[5] = cs;    T[9]  = 0.0f;  T[13] = p != nullptr ? p[1] : 0.0f;
    T[2] = 0.0f;  T[6] = 0.0f;  T[10] = 1.0f;  T[14] = p != nullptr ? p[2] : 0.0f;
    T[3] = 0.0f;  T[7] = 0.0f;  T[11] = 0.0f;  T[15] = 1.0f;
}

void axis2T(float* T, float* w, float &a, float* p) {
    const float cs = cos(DEG2RAD(a));
    const float sn = sin(DEG2RAD(a));

    normVec3(w); float t[3];
    t[0] = (1 - cs) * w[0];
    t[1] = (1 - cs) * w[1];
    t[2] = (1 - cs) * w[2];

    T[0] = cs + t[0] * w[0];          T[4] = t[0] * w[1] + sn * w[2];   T[8]  = t[0] * w[2] - sn * w[1];   T[12] = p != nullptr ? p[0] : 0.0f;
    T[1] = t[0] * w[1] - sn * w[2];   T[5] = cs + t[1] * w[1];          T[9]  = t[1] * w[2] + sn * w[0];   T[13] = p != nullptr ? p[1] : 0.0f;
    T[2] = t[0] * w[2] + sn * w[1];   T[6] = t[1] * w[2] - sn * w[0];   T[10] = cs + t[2] * w[2];          T[14] = p != nullptr ? p[2] : 0.0f;
    T[3] = 0.0f;                      T[7] = 0.0f;                      T[11] = 0.0f;                      T[15] = 1.0f;
}

void perspective(float* P, float& fov, float& aspect, float& znear, float &zfar) {
    float tanHalfFov = tan(DEG2RAD(fov) / 2.0f);
    float zrange = znear - zfar;

    P[0] = 1 / (aspect * tanHalfFov);  P[4] = 0.0f;             P[8]  = 0.0f;                     P[12] = 0.0f;
    P[1] = 0.0f;                       P[5] = 1 / tanHalfFov;   P[9]  = 0.0f;                     P[13] = 0.0f;
    P[2] = 0.0f;                       P[6] = 0.0f;             P[10] = (zfar + znear) / zrange;  P[14] = (2.0f * zfar * znear) / zrange;
    P[3] = 0.0f;                       P[7] = 0.0f;             P[11] = -1.0f;                    P[15] = 0.0f;
}

void transcale(float* T, float* p, float* s) {
    T[0] = s[0];  T[4] = 0.0f;  T[8]  = 0.0f;  T[12] = p[0];
    T[1] = 0.0f;  T[5] = s[1];  T[9]  = 0.0f;  T[13] = p[1];
    T[2] = 0.0f;  T[6] = 0.0f;  T[10] = s[2];  T[14] = p[2];
    T[3] = 0.0f;  T[7] = 0.0f;  T[11] = 0.0f;  T[15] = 1.0f;
}

void decompose(float* T, float* p, float* s, float* e) {
    p[0] = T[12];
    p[1] = T[13];
    p[2] = T[14];

    s[0] = sqrt(T[0] * T[0] + T[1] * T[1] + T[2] * T[2]);
    s[1] = sqrt(T[4] * T[4] + T[5] * T[5] + T[6] * T[6]);
    s[2] = sqrt(T[8] * T[8] + T[9] * T[9] + T[10] * T[10]);

    float R[9];
    R[0] = T[0] / s[0]; R[3] = T[4] / s[1]; R[6] = T[8] / s[2];
    R[1] = T[1] / s[0]; R[4] = T[5] / s[1]; R[7] = T[9] / s[2];
    R[2] = T[2] / s[0]; R[5] = T[6] / s[1]; R[8] = T[10] / s[2];

    e[1] = asin(-R[2]);
    if (cos(e[1]) != 0) {
        e[0] = atan2(R[5], R[8]);
        e[2] = atan2(R[1], R[0]);
    } else {
        e[0] = atan2(-R[7], R[4]);
        e[2] = 0;
    }
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

void invert3(float *res, float* m) {
    float det = m[0] * (m[4] * m[8] - m[5] * m[7]) -
                m[1] * (m[3] * m[8] - m[5] * m[6]) +
                m[2] * (m[3] * m[7] - m[4] * m[6]);

    if (fabs(det) < MINVAL) {
        identity3(res); // Matrix is not invertible
        return;
    }

    float invDet = 1.0f / det;

    res[0] = (m[4] * m[8] - m[5] * m[7]) * invDet;
    res[1] = (m[2] * m[7] - m[1] * m[8]) * invDet;
    res[2] = (m[1] * m[5] - m[2] * m[4]) * invDet;
    res[3] = (m[5] * m[6] - m[3] * m[8]) * invDet;
    res[4] = (m[0] * m[8] - m[2] * m[6]) * invDet;
    res[5] = (m[2] * m[3] - m[0] * m[5]) * invDet;
    res[6] = (m[3] * m[7] - m[4] * m[6]) * invDet;
    res[7] = (m[1] * m[6] - m[0] * m[7]) * invDet;
    res[8] = (m[0] * m[4] - m[1] * m[3]) * invDet;
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
    det = 1.0f / det; for (uint32_t i = 0; i < 16; i++) res[i] = res[i] * det;
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

void transpose3(float* m) {
    std::swap(m[1], m[3]);
    std::swap(m[2], m[6]);
    std::swap(m[5], m[7]);
}

void transpose4(float* m) {
    std::swap(m[1], m[4]);
    std::swap(m[2], m[8]);
    std::swap(m[3], m[12]);
    std::swap(m[6], m[9]);
    std::swap(m[7], m[13]);
    std::swap(m[11], m[14]);
}

void identity3(float* m) {
    memset(m, 0, 9 * sizeof(float));
    m[0] = 1.0f;
    m[5] = 1.0f;
    m[7] = 1.0f;
}

void identity4(float* m) {
    memset(m, 0, 16 * sizeof(float));
    m[0] = 1.0f;
    m[4] = 1.0f;
    m[10] = 1.0f;
    m[15] = 1.0f;
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

ImVec4 mulmatvec4(float* m, const ImVec4& v) {
    const float x = m[0] * v.x + m[4] * v.y + m[8] * v.z + m[12] * v.w;
    const float y = m[1] * v.x + m[5] * v.y + m[9] * v.z + m[13] * v.w;
    const float z = m[2] * v.x + m[6] * v.y + m[10] * v.z + m[14] * v.w;
    const float w = m[3] * v.x + m[7] * v.y + m[11] * v.z + m[15] * v.w;
    return { x, y, z, w };
}

void printMat4(float* m) {
    std::cout << std::fixed << std::setprecision(2);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            LOG("{0} ", m[j * 4 + i]);
        } std::cout << '\n';
    } std::cout << '\n';
}

void printMat3(float* m) {
    std::cout << std::fixed << std::setprecision(2);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            LOG("{0} ", m[j * 3 + i]);
        } std::cout << '\n';
    } std::cout << '\n';
}

void printVec4(float* v) {
    std::cout << std::fixed << std::setprecision(2);
    LOG("{0} {1} {2} {3}", v[0], v[1], v[2], v[3]);
}

void printVec3(float* v) {
    std::cout << std::fixed << std::setprecision(2);
    LOG("{0} {1} {2}", v[0], v[1], v[2]);
}

void vizgraph(std::unordered_map<std::string, std::vector<std::string>> &graph) {
    for (auto &[name, vec] : graph) {
        LOG("{0} => (", name);
        for (auto &v : graph[name]) {
            LOG("{0} ", v);
        } LOG(") \n");
    }
}

}