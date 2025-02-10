#include "Vector.h"

namespace Skynet
{
    namespace Math
    {
        
        void AddVec3(float* res, float* vec1, float* vec2) {
            res[0] = vec1[0] + vec2[0];
            res[1] = vec1[1] + vec2[1];
            res[2] = vec1[2] + vec2[2];
        }

        void SubVec3(float* res, float* vec1, float* vec2) {
            res[0] = vec1[0] - vec2[0];
            res[1] = vec1[1] - vec2[1];
            res[2] = vec1[2] - vec2[2];
        }

        void AddVecS3(float* res, float* vec1, float s) {
            res[0] = vec1[0] + s;
            res[1] = vec1[1] + s;
            res[2] = vec1[2] + s;
        }

        void SubVecS3(float* res, float* vec, float s) {
            res[0] = vec[0] - s;
            res[1] = vec[1] - s;
            res[2] = vec[2] - s;
        }

        void MulVecS3(float* res, float* vec, float s) {
            res[0] = vec[0] * s;
            res[1] = vec[1] * s;
            res[2] = vec[2] * s;
        }

        void DivVecS3(float* res, float* vec, float s) {
            ASSERT(s > MINVAL, "Divide by zero");
            res[0] = vec[0] / s;
            res[1] = vec[1] / s;
            res[2] = vec[2] / s;
        }

        void AddScl3(float* res, float* vec, float s) {
            res[0] += vec[0] * s;
            res[1] += vec[1] * s;
            res[2] += vec[2] * s;
        }

        void AddScl4(float* res, float* vec, float s) {
            res[0] += vec[0] * s;
            res[1] += vec[1] * s;
            res[2] += vec[2] * s;
            res[3] += vec[3] * s;
        }

        void NormVec(float* vec, int n, float* norm) {
            float norm_ = 0.0f;
            for (int i = 0; i < n; i++) {
                norm_ += vec[i] * vec[i];
            }
            norm_ = sqrt(norm_);
            if (norm_ < MINVAL) {
                for (int i = 0; i < n; i++) {
                    vec[i] = (i == 0) ? 1.0f : 0.0f;
                }
                norm_ = 1.0f;
            } else {
                float normInv = 1 / n;
                for (int i = 0; i < n; i++) {
                    vec[i] *= normInv;
                }
            }
            if (norm) *norm = norm_;
        }

        void NormVec3(float* vec, float* norm) {
            float n = sqrt(vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2]);
            if (n < MINVAL) {
                vec[0] = 1; vec[1] = 0; vec[2] = 0;
                n = 1.0f;
            }
            float normInv = 1 / n;
            vec[0] *= normInv;
            vec[1] *= normInv;
            vec[2] *= normInv;
            if (norm) *norm = n;
        }

        void NormVec4(float* vec, float* norm) {
            float n = sqrt(vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2] + vec[3]*vec[3]);
            if (n < MINVAL) {
                vec[0] = 1; vec[1] = 0; vec[2] = 0; vec[3] = 0;
                n = 1.0f;
            }
            float normInv = 1/n;
            vec[0] *= normInv;
            vec[1] *= normInv;
            vec[2] *= normInv;
            vec[3] *= normInv;
            if (norm) *norm = n;
        }


        void NormVecS(float &norm, float* vec, int n) {
            norm = 0.0f;
            for (int i = 0; i < n; i++) {
                norm += vec[i] * vec[i];
            }
            norm = sqrt(norm);
            if (norm < MINVAL) {
                norm = 1.0f;
            }
        }

        void NormVecS3(float &norm, float* v) {
            norm = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
            if (norm < MINVAL) norm = 1.0f;
        }

        void NormVecS4(float &norm, float* v) {
            norm = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2] + v[3]*v[3]);
            if (norm < MINVAL) norm = 1.0f;
        }

        void CrossVec3(float* res, float* vec1, float* vec2) {
            res[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
            res[1] = vec1[2] * vec2[0] - vec1[0] * vec2[2];
            res[2] = vec1[0] * vec2[1] - vec1[1] * vec2[0];
        }

        void DotVec3(float &res, float* vec1, float* vec2) {
            res = vec1[0] * vec2[0] + vec1[1] * vec2[1] + vec1[2] * vec2[2];
        }

        void Copy3(float* res, float* vec) {
            res[0] = vec[0];
            res[1] = vec[1];
            res[2] = vec[2];
        }

        void Zero3(float* vec) {
            vec[0] = 0.0f;
            vec[1] = 0.0f;
            vec[2] = 0.0f;
        }
    
    } // namespace Math

} // namespace Skynet