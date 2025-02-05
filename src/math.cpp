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
    const float cs = cos(-a);
    const float sn = sin(-a);

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

void translate(float* T, float* w, float &a) {
    T[12] = w[0] * a;
    T[13] = w[1] * a;
    T[14] = w[2] * a;
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

void decomposeTw(float* T, float* p, float* q) {
    p[0] = T[12];
    p[1] = T[13];
    p[2] = T[14];

    float trace = T[0] + T[5] + T[10];
    if (trace > 0.0f) {
        float s = 0.5f / sqrtf(trace + 1.0f);
        q[0] = 0.25f / s; // w
        q[1] = (T[6] - T[9]) * s;
        q[2] = (T[8] - T[2]) * s;
        q[3] = (T[1] - T[4]) * s;
    } else {
        if (T[0] > T[5] && T[0] > T[6]) {
            float s = 2.0f * sqrtf(1.0f + T[0] - T[5] - T[6]);
            q[0] = (T[6] - T[9]) / s;
            q[1] = 0.25f * s;
            q[2] = (T[4] + T[1]) / s;
            q[3] = (T[8] + T[2]) / s;
        } else if (T[5] > T[6]) {
            float s = 2.0f * sqrtf(1.0f + T[5] - T[0] - T[6]);
            q[0] = (T[8] - T[2]) / s;
            q[1] = (T[4] + T[1]) / s;
            q[2] = 0.25f * s;
            q[3] = (T[9] + T[6]) / s;
        } else {
            float s = 2.0f * sqrtf(1.0f + T[6] - T[0] - T[5]);
            q[0] = (T[1] - T[4]) / s;
            q[1] = (T[8] + T[2]) / s;
            q[2] = (T[9] + T[6]) / s;
            q[3] = 0.25f * s;
        }
    }
}


void mulMatVec(float* res, float* mat, float* vec, int n, int m) {
    for (int i = 0; i < n; i++) {
        res[i] = 0.0f;
        for (int j = 0; j < m; j++) {
            res[i] += mat[j * m + i] * vec[j];
        }
    }
}

void mulMatVec4(float* res, float* mat, float* vec) {
    res[0] = mat[0] * vec[0] + mat[4] * vec[1] + mat[8] * vec[2] + mat[12];
    res[1] = mat[1] * vec[0] + mat[5] * vec[1] + mat[9] * vec[2] + mat[13];
    res[2] = mat[2] * vec[0] + mat[6] * vec[1] + mat[10] * vec[2] + mat[14];
    res[3] = mat[3] * vec[0] + mat[7] * vec[1] + mat[11] * vec[2] + mat[15];
}

void rotMatVec3(float* res, float* mat, float* vec) {
    res[0] = mat[0] * vec[0] + mat[4] * vec[1] + mat[8] * vec[2];
    res[1] = mat[1] * vec[0] + mat[5] * vec[1] + mat[9] * vec[2];
    res[2] = mat[2] * vec[0] + mat[6] * vec[1] + mat[10] * vec[2];
}


void matmul(float* product, float* jac, float* jac_T, int n, int m, int q) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < q; j++) {
            product[i * q + j]= 0.0f;
            for (int k = 0; k < m; k++) {
                product[i * q + j] += jac[i * m + k] * jac_T[k * q + j];
            }
        }
    }
}


void matmulsq(float* res, float* m1, float* m2, int n) {
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

void transpose(float* mat_T, float* mat, int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            mat_T[j * n + i] = mat[i * m + j];
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

void addVec3(float* res, float* v1, float* v2) {
    res[0] = v1[0] + v2[0];
    res[1] = v1[1] + v2[1];
    res[2] = v1[2] + v2[2];
}

void addVecS3(float* res, float* v1, float &a) {
    res[0] = v1[0] + a;
    res[1] = v1[1] + a;
    res[2] = v1[2] + a;
}

void subVec3(float* res, float* v1, float* v2) {
    res[0] = v1[0] - v2[0];
    res[1] = v1[1] - v2[1];
    res[2] = v1[2] - v2[2];
}

void mulVecS3(float* res, float* v, float &s) {
    res[0] = v[0] * s;
    res[1] = v[1] * s;
    res[2] = v[2] * s;
}

void divVecS3(float* res, float* v, float &s) {
    ASSERT(s > MINVAL, "Divide by zero");
    res[0] = v[0] / s;
    res[1] = v[1] / s;
    res[2] = v[2] / s;
}

void addScl3(float* res, float* vec, float s) {
    res[0] += vec[0] * s;
    res[1] += vec[1] * s;
    res[2] += vec[2] * s;
}

float normVec3(float* v) {
    float norm = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    if (norm < MINVAL) {
        v[0] = 1; v[1] = 0; v[2] = 0;
    }
    float normInv = 1/norm;
    v[0] *= normInv;
    v[1] *= normInv;
    v[2] *= normInv;
    return norm;
}

float normVec(float* v, int n) {
    float norm = 0.0f;
    for (int i = 0; i < n; i++) {
        norm += v[i] * v[i];
    }
    return sqrt(norm);
}

float normVec4(float* v) {
    float norm = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2] + v[3]*v[3]);
    if (norm < MINVAL) {
        v[0] = 1; v[1] = 0;
        v[2] = 0; v[3] = 0;
        return 1;
    }
    float normInv = 1/norm;
    v[0] *= normInv;
    v[1] *= normInv;
    v[2] *= normInv;
    v[3] *= normInv;
    return norm;
}

void cross3(float* res, float* v1, float* v2) {
    res[0] = v1[1] * v2[2] - v1[2] * v2[1];
    res[1] = v1[2] * v2[0] - v1[0] * v2[2];
    res[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

float dotVec3(float* v1, float* v2) {
    return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

void zero3(float* v) {
    v[0] = 0.0f;
    v[1] = 0.0f;
    v[2] = 0.0f;
}

void copy3(float* res, const float* v) {
    res[0] = v[0];
    res[1] = v[1];
    res[2] = v[2];
}

void rotVecQuat(float res[3], const float vec[3], const float quat[4]) {
    if (vec[0] == 0 && vec[1] == 0 && vec[2] == 0) {
        zero3(res);
    }

    else if (quat[0] == 1 && quat[1] == 0 && quat[2] == 0 && quat[3] == 0) {
        copy3(res, vec);
    }

    else {
        float tmp[3] = {
            quat[0]*vec[0] + quat[2]*vec[2] - quat[3]*vec[1],
            quat[0]*vec[1] + quat[3]*vec[0] - quat[1]*vec[2],
            quat[0]*vec[2] + quat[1]*vec[1] - quat[2]*vec[0]
        };

        res[0] = vec[0] + 2 * (quat[2]*tmp[2] - quat[3]*tmp[1]);
        res[1] = vec[1] + 2 * (quat[3]*tmp[0] - quat[1]*tmp[2]);
        res[2] = vec[2] + 2 * (quat[1]*tmp[1] - quat[2]*tmp[0]);
    }
}

void clamp(float &a, float* range) {
    if (a > range[1]) a = range[1];
    else if (a < range[0]) a = range[0];
}

void invert(float* m, int n) {
    std::vector<int> indxc(n), indxr(n), ipiv(n);
    for (int i = 0; i < n; i++) ipiv[i] = 0;

    for (int i = 0; i < n; i++) {
        int irow = -1, icol = -1;
        float big = 0.0f;

        for (int j = 0; j < n; j++) {
            if (ipiv[j] != 1) {
                for (int k = 0; k < n; k++) {
                    if (ipiv[k] == 0) {
                        if (fabs(m[j * n + k]) >= big) {
                            big = fabs(m[j * n + k]);
                            irow = j;
                            icol = k;
                        }
                    }
                }
            }
        }

        ++(ipiv[icol]);

        if (irow != icol) {
            for (int l = 0; l < n; l++) std::swap(m[irow * n + l], m[icol * n + l]);
        }

        indxr[i] = irow;
        indxc[i] = icol;

        if (m[icol * n + icol] == 0.0f) {
            std::cerr << "Singular matrix in invert\n";
            return;
        }

        float pivinv = 1.0f / m[icol * n + icol];
        m[icol * n + icol] = 1.0f;

        for (int l = 0; l < n; l++) m[icol * n + l] *= pivinv;

        for (int ll = 0; ll < n; ll++) {
            if (ll != icol) {
                float dum = m[ll * n + icol];
                m[ll * n + icol] = 0.0f;
                for (int l = 0; l < n; l++) m[ll * n + l] -= m[icol * n + l] * dum;
            }
        }
    }

    for (int l = n - 1; l >= 0; l--) {
        if (indxr[l] != indxc[l]) {
            for (int k = 0; k < n; k++) {
                std::swap(m[k * n + indxr[l]], m[k * n + indxc[l]]);
            }
        }
    }
}


// void pseudoinvert(float* J, int n, int m) {
//     if (n < m) {
//         float J_T[m * n];
//         std::copy(J, J + (n * m), J_T);
//         transpose(J_T, );

//         float product[m * n];        
//         matmul(product, J, J_T, n, m, n);
//         invert(product, n);
//         matmul(J, J_T, product, m, n, n);
//     }
// }


ImVec4 mulmatvec4(float* m, const ImVec4& v) {
    const float x = m[0] * v.x + m[4] * v.y + m[8] * v.z + m[12] * v.w;
    const float y = m[1] * v.x + m[5] * v.y + m[9] * v.z + m[13] * v.w;
    const float z = m[2] * v.x + m[6] * v.y + m[10] * v.z + m[14] * v.w;
    const float w = m[3] * v.x + m[7] * v.y + m[11] * v.z + m[15] * v.w;
    return { x, y, z, w };
}

void printMat(float* mat, int n, int m) {
    std::ostringstream o;
    o << std::fixed << std::setprecision(3);
    for (int i=0; i<n; i++) {
        for (int j=0; j<m; j++) {
            o << mat[i * m + j] << ' ';
        } o << '\n';
    }
    LOG("{}", o.str());
}

void printMat4(float* m) {
    std::ostringstream o;
    o << std::fixed << std::setprecision(3);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            o << m[j * 4 + i] << ' ';
        } o << '\n';
    }
    LOG("{}", o.str());
}

void printMat3(float* m) {
    std::ostringstream o;
    o << std::fixed << std::setprecision(3);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            o << m[j * 3 + i] << ' ';
        } o << '\n';
    }
    LOG("{}", o.str());
}

void printVec4(float* v) {
    std::ostringstream o;
    o << std::fixed << std::setprecision(3);
    o << v[0] << ' ' << v[1] << ' ' << v[2] << ' ' << v[3] << '\n';
    LOG("{}", o.str());
}
    
void printVec3(float* v) {
    std::ostringstream o;
    o << std::fixed << std::setprecision(7);
    o << v[0] << ' ' << v[1] << ' ' << v[2] << " \n";
    LOG("{}", o.str());
}

void printVec(float* v, int n) {
    std::ostringstream o;
    o << std::fixed << std::setprecision(3);
    for (int i = 0; i < n; i++) {
        o << v[i] << ' ';
    } o << '\n';
    LOG("{}", o.str());
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