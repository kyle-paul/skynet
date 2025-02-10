#ifndef SKYNET_MATH_GRAPHICS_H
#define SKYNET_MATH_GRAPHICS_H

#include "Constants.h"

namespace Skynet
{
    namespace Math
    {
        void Quat2T(float* T, float* q, float* p = nullptr);
        void Euler2T(float* T, float* e, float* p = nullptr);
        void AxisX2T(float* T, float* w, float &a, float* p = nullptr);
        void AxisY2T(float* T, float* w, float &a, float* p = nullptr);
        void AxisZ2T(float* T, float* w, float &a, float* p = nullptr);
        void Axis2T(float* T, float* w, float &a, float* p = nullptr);
        void Perspective(float* P, float& fov, float& aspect, float& znear, float &zfar);
        void Transcale(float* T, float* p, float* s);
        void Translate(float* T, float* w, float &a);
        void Decompose(float* T, float* p, float* s, float* e);
        void DecomposeTw(float* T, float* p, float* q);
        void RotVecQuat(float* res, float* vec, float* quat);
    
    } // namespace Math

} // namespace Skynet



#endif // SKYNET_MATH_GRAPHICS_H