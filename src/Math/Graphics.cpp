#include "Graphics.h"
#include "Vector.h"

namespace Skynet
{
    namespace Math
    {
        void Quat2T(float* T, float* q, float* p) {
            NormVec4(q);
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

        void Euler2T(float* T, float* e, float *p) {
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


        void AxisX2T (float* T, float* e, float &a, float* p) {
            const float cs = cos(DEG2RAD(a));
            const float sn = sin(DEG2RAD(a));

            T[0] = 1.0f;  T[0] = 0.0f;  T[0] = 0.0f;   T[12] = p != nullptr ? p[0] : 0.0f;
            T[1] = 0.0f;  T[1] = cs;    T[1] = -sn;    T[13] = p != nullptr ? p[1] : 0.0f;
            T[2] = 0.0f;  T[2] = sn;    T[2] = cs;     T[14] = p != nullptr ? p[2] : 0.0f;
            T[3] = 0.0f;  T[3] = 0.0f;  T[3] = 0.0f;   T[3] = 1.0f;
        }

        void AxisY2T (float* T, float* e, float &a, float* p) {
            const float cs = cos(DEG2RAD(a));
            const float sn = sin(DEG2RAD(a));
            
            T[0] = cs;    T[4] = 0.0f;  T[8]  = -sn;   T[12] = p != nullptr ? p[0] : 0.0f;
            T[1] = 0.0f;  T[5] = 1.0f;  T[9]  = 0.0f;  T[13] = p != nullptr ? p[1] : 0.0f;
            T[2] = sn;    T[6] = 0.0f;  T[10] = cs;    T[14] = p != nullptr ? p[2] : 0.0f;
            T[3] = 0.0f;  T[7] = 0.0f;  T[11] = 0.0f;  T[15] = 1.0f;
        }

        void AxisZ2T (float* T, float* e, float &a, float* p) {
            const float cs = cos(DEG2RAD(a));
            const float sn = sin(DEG2RAD(a));
            
            T[0] = cs;    T[4] = -sn;   T[8]  = 0.0f;  T[12] = p != nullptr ? p[0] : 0.0f;
            T[1] = sn;    T[5] = cs;    T[9]  = 0.0f;  T[13] = p != nullptr ? p[1] : 0.0f;
            T[2] = 0.0f;  T[6] = 0.0f;  T[10] = 1.0f;  T[14] = p != nullptr ? p[2] : 0.0f;
            T[3] = 0.0f;  T[7] = 0.0f;  T[11] = 0.0f;  T[15] = 1.0f;
        }

        void Axis2T(float* T, float* w, float &a, float* p) {
            const float cs = cos(-a);
            const float sn = sin(-a);

            NormVec3(w); float t[3];
            t[0] = (1 - cs) * w[0];
            t[1] = (1 - cs) * w[1];
            t[2] = (1 - cs) * w[2];

            T[0] = cs + t[0] * w[0];          T[4] = t[0] * w[1] + sn * w[2];   T[8]  = t[0] * w[2] - sn * w[1];   T[12] = p != nullptr ? p[0] : 0.0f;
            T[1] = t[0] * w[1] - sn * w[2];   T[5] = cs + t[1] * w[1];          T[9]  = t[1] * w[2] + sn * w[0];   T[13] = p != nullptr ? p[1] : 0.0f;
            T[2] = t[0] * w[2] + sn * w[1];   T[6] = t[1] * w[2] - sn * w[0];   T[10] = cs + t[2] * w[2];          T[14] = p != nullptr ? p[2] : 0.0f;
            T[3] = 0.0f;                      T[7] = 0.0f;                      T[11] = 0.0f;                      T[15] = 1.0f;
        }

        void Perspective(float* P, float& fov, float& aspect, float& znear, float &zfar) {
            float tanHalfFov = tan(DEG2RAD(fov) / 2.0f);
            float zrange = znear - zfar;

            P[0] = 1 / (aspect * tanHalfFov);  P[4] = 0.0f;             P[8]  = 0.0f;                     P[12] = 0.0f;
            P[1] = 0.0f;                       P[5] = 1 / tanHalfFov;   P[9]  = 0.0f;                     P[13] = 0.0f;
            P[2] = 0.0f;                       P[6] = 0.0f;             P[10] = (zfar + znear) / zrange;  P[14] = (2.0f * zfar * znear) / zrange;
            P[3] = 0.0f;                       P[7] = 0.0f;             P[11] = -1.0f;                    P[15] = 0.0f;
        }

        void Transcale(float* T, float* p, float* s) {
            T[0] = s[0];  T[4] = 0.0f;  T[8]  = 0.0f;  T[12] = p[0];
            T[1] = 0.0f;  T[5] = s[1];  T[9]  = 0.0f;  T[13] = p[1];
            T[2] = 0.0f;  T[6] = 0.0f;  T[10] = s[2];  T[14] = p[2];
            T[3] = 0.0f;  T[7] = 0.0f;  T[11] = 0.0f;  T[15] = 1.0f;
        }

        void Translate(float* T, float* w, float &a) {
            T[12] = w[0] * a;
            T[13] = w[1] * a;
            T[14] = w[2] * a;
        }

        void Decompose(float* T, float* p, float* s, float* e) {
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

        void DecomposeTw(float* T, float* p, float* q) {
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

        void RotVecQuat(float* res, float* vec, float* quat) {
            if (vec[0] == 0 && vec[1] == 0 && vec[2] == 0) {
                Zero3(res);
            }

            else if (quat[0] == 1 && quat[1] == 0 && quat[2] == 0 && quat[3] == 0) {
                Copy3(res, vec);
            }

            else {
                float t[3] = {
                    quat[0]*vec[0] + quat[2]*vec[2] - quat[3]*vec[1],
                    quat[0]*vec[1] + quat[3]*vec[0] - quat[1]*vec[2],
                    quat[0]*vec[2] + quat[1]*vec[1] - quat[2]*vec[0]
                };

                res[0] = vec[0] + 2 * (quat[2]*t[2] - quat[3]*t[1]);
                res[1] = vec[1] + 2 * (quat[3]*t[0] - quat[1]*t[2]);
                res[2] = vec[2] + 2 * (quat[1]*t[1] - quat[2]*t[0]);
            }
        }


    } // namespace Math

} // namepsace Skynet