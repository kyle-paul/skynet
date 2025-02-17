#ifndef SKYNET_MATH_MATRIX_H
#define SKYNET_MATH_MATRIX_H

#include "Constants.h"
#include "imgui.h"

namespace Skynet
{
    namespace Math
    {
        /* matrix-matrix operations */
        void Matmul(float* res, float* mat1, float* mat2, int m, int n, int q);
        void Matmul3(float* res, float* mat1, float* mat2);
        void Matmul4(float* res, float* mat1, float* mat2);
        void Matmul4(float* res, const float* m1, const float* m2);

        void Invert(float* mat, int n);
        void Invert3(float* res, float* mat);
        void Invert4(float* res, float* mat);

        void Transpose(float* res, float* mat, int n, int m);
        void Transpose3(float* mat);
        void Transpose4(float* mat);

        void Identity3(float* mat);
        void Identity4(float* mat);

        /* matrix-vector operations */
        void MulMatVec(float* res, float* mat, float* vec, int n, int m);
        void MulMatVec4(float* res, float* mat, float* vec);
        void RotMatVec3(float* res, float* mat, float* vec);
        void MulMatVec3R(float* res, float* mat, float* vec);

        /* other oeprations */ 
        void SkewMatrix(float* res, float* vec);
        ImVec4 MulMatVec4(float* m, const ImVec4& v);
    
    } // namespace Math

} // namespace Skynet



#endif // SKYNET_MATH_MATRIX_H