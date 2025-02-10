#ifndef SKYNET_MATH_VISUALIZE_H
#define SKYNET_MATH_VISUALIZE_H

#include "Constants.h"

namespace Skynet
{
    namespace Math
    {
        void PrintMat(float* mat, int n, int m);
        void PrintMat3(float* m);
        void PrintMat4(float* m);
        void PrintVec3(float* v);
        void PrintVec4(float* v);
        void PrintVec(float* v, int n);
    
    } // namespace Math

} // namespace Skynet



#endif // SKYNET_MATH_VISUALIZE_H