#include "Matrix.h"

namespace Skynet
{
    namespace Math
    {

        /* matrix operations */
        void Matmul(float* res, float* mat1, float* mat2, int n, int m, int q) {
            memset(res, 0, n * q * sizeof(float));
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < q; j++) {
                    for (int k = 0; k < m; k++) {
                        res[i * q + j] += mat1[i * m + k] * mat2[k * q + j];
                    }
                }
            }
        }

        void Matmul4(float* res, float* m1, float* m2) {
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

        void Matmul3(float *res, float* m1, float* m2) {
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

        void Invert(float* mat, int n) {

            for (int i = 0; i < n * n; i++) {
                if (mat[i] != 0.0f) {
                    break;
                }
                if (i == n * n - 1) {
                    return;
                }
            }

            std::vector<int> indxc(n), indxr(n), ipiv(n);
            for (int i = 0; i < n; i++) ipiv[i] = 0;

            for (int i = 0; i < n; i++) {
                int irow = -1, icol = -1;
                float big = 0.0f;

                for (int j = 0; j < n; j++) {
                    if (ipiv[j] != 1) {
                        for (int k = 0; k < n; k++) {
                            if (ipiv[k] == 0) {
                                if (fabs(mat[j * n + k]) >= big) {
                                    big = fabs(mat[j * n + k]);
                                    irow = j;
                                    icol = k;
                                }
                            }
                        }
                    }
                }

                ++(ipiv[icol]);

                if (irow != icol) {
                    for (int l = 0; l < n; l++) std::swap(mat[irow * n + l], mat[icol * n + l]);
                }

                indxr[i] = irow;
                indxc[i] = icol;

                if (mat[icol * n + icol] == 0.0f) {
                    std::cerr << "Singular matrix in invert\n";
                    return;
                }

                float pivinv = 1.0f / mat[icol * n + icol];
                mat[icol * n + icol] = 1.0f;

                for (int l = 0; l < n; l++) mat[icol * n + l] *= pivinv;

                for (int ll = 0; ll < n; ll++) {
                    if (ll != icol) {
                        float dum = mat[ll * n + icol];
                        mat[ll * n + icol] = 0.0f;
                        for (int l = 0; l < n; l++) mat[ll * n + l] -= mat[icol * n + l] * dum;
                    }
                }
            }

            for (int l = n - 1; l >= 0; l--) {
                if (indxr[l] != indxc[l]) {
                    for (int k = 0; k < n; k++) {
                        std::swap(mat[k * n + indxr[l]], mat[k * n + indxc[l]]);
                    }
                }
            }
        }

        void Invert3(float *res, float* m) {
            float det = m[0] * (m[4] * m[8] - m[5] * m[7]) -
                        m[1] * (m[3] * m[8] - m[5] * m[6]) +
                        m[2] * (m[3] * m[7] - m[4] * m[6]);

            if (fabs(det) < MINVAL) {
                Identity3(res);
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

        void Invert4(float* res, float* m) {
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

        void Transpose(float* res, float* mat, int n, int m) {
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < m; j++) {
                    res[j * n + i] = mat[i * m + j];
                }
            }
        }

        void Transpose3(float* mat) {
            std::swap(mat[1], mat[3]);
            std::swap(mat[2], mat[6]);
            std::swap(mat[5], mat[7]);
        }

        void Transpose4(float* mat) {
            std::swap(mat[1], mat[4]);
            std::swap(mat[2], mat[8]);
            std::swap(mat[3], mat[12]);
            std::swap(mat[6], mat[9]);
            std::swap(mat[7], mat[13]);
            std::swap(mat[11], mat[14]);
        }

        void Identity3(float* mat) {
            memset(mat, 0, 9 * sizeof(float));
            mat[0] = 1.0f;
            mat[5] = 1.0f;
            mat[7] = 1.0f;
        }

        void Identity4(float* mat) {
            memset(mat, 0, 16 * sizeof(float));
            mat[0] = 1.0f;
            mat[4] = 1.0f;
            mat[10] = 1.0f;
            mat[15] = 1.0f;
        }


        /* matrix-vector operations */
        void MulMatVec(float* res, float* mat, float* vec, int n, int m) {
            memset(res, 0, n * sizeof(float));
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < m; j++) {
                    res[i] += mat[j * n + i] * vec[j];
                }
            }
        }

        void MulMatVec4(float* res, float* mat, float* vec) {
            res[0] = mat[0] * vec[0] + mat[4] * vec[1] + mat[8] * vec[2] + mat[12];
            res[1] = mat[1] * vec[0] + mat[5] * vec[1] + mat[9] * vec[2] + mat[13];
            res[2] = mat[2] * vec[0] + mat[6] * vec[1] + mat[10] * vec[2] + mat[14];
            res[3] = mat[3] * vec[0] + mat[7] * vec[1] + mat[11] * vec[2] + mat[15];
        }

        void RotMatVec3(float* res, float* mat, float* vec) {
            res[0] = mat[0] * vec[0] + mat[4] * vec[1] + mat[8] * vec[2];
            res[1] = mat[1] * vec[0] + mat[5] * vec[1] + mat[9] * vec[2];
            res[2] = mat[2] * vec[0] + mat[6] * vec[1] + mat[10] * vec[2];
        }

        void MulMatVec3R(float* res, float* mat, float* vec) {
            res[0] = mat[0] * vec[0] + mat[1] * vec[1] + mat[2] * vec[2];
            res[1] = mat[3] * vec[0] + mat[4] * vec[1] + mat[5] * vec[2];
            res[2] = mat[5] * vec[6] + mat[7] * vec[1] + mat[8] * vec[2];
        }
        
        void SkewMatrix(float* res, float* vec)
        {
            res[0] = 0.0f;    res[1] = -vec[2]; res[2] = vec[1];
            res[3] = vec[2];  res[4] = 0.0f;    res[5] = -vec[0];
            res[6] = -vec[1]; res[7] = vec[0];  res[8] = 0.0f; 
        }

        ImVec4 MulMatVec4(float* m, const ImVec4& v) {
            const float x = m[0] * v.x + m[4] * v.y + m[8] * v.z + m[12] * v.w;
            const float y = m[1] * v.x + m[5] * v.y + m[9] * v.z + m[13] * v.w;
            const float z = m[2] * v.x + m[6] * v.y + m[10] * v.z + m[14] * v.w;
            const float w = m[3] * v.x + m[7] * v.y + m[11] * v.z + m[15] * v.w;
            return { x, y, z, w };
        }

    
    } // namespace Math

} // namespace Skynet