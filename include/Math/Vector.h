#ifndef SKYNET_MATH_VECTOR_H
#define SKYNET_MATH_VECTOR_H

#include "Constants.h"

namespace Skynet
{
    namespace Math
    {
        void AddVec3(float* res, float* vec1, float* vec2);
        void SubVec3(float* res, float* vec1, float* vec2);

        void AddVecS3(float* res, float* vec, float s);
        void SubVecS3(float* res, float* vec, float s);
        void MulVecS3(float* res, float* vec, float s);
        void DivVecS3(float* res, float* vec, float s);

        void AddScl3(float* res, float* vec, float s);
        void AddScl4(float* res, float* vec, float s);

        void NormVec(float* v, int n, float* norm = nullptr);
        void NormVec3(float* vec, float* norm = nullptr);
        void NormVec4(float* vec, float* norm = nullptr);
        void NormVecS(float &norm, float* v, int n);
        void NormVecS3(float &norm, float* v);
        void NormVecS4(float &norm, float* v);

        void DotVec3(float &res, float* v1, float* v2);
        void CrossVec3(float* res, float* v1, float* v2);

        void Copy3(float* res, float* vec);
        void Zero3(float* vec);
    
    } // namespace Math

} // namespace Skynet



#endif // SKYNET_MATH_VECTOR_H