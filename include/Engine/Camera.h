#ifndef SKYNET_CAMERA_H
#define SKYNET_CAMERA_H

#include "Math.h"

namespace Skynet
{
    class Camera
    {
    public:
        Camera();
        ~Camera();

        void UpdateView();
        void UpdateProjection();
        void UpdateProjView();

        inline float* GetView() { return V; }
        inline float* GetProjection() { return P; }
        inline float* GetProjView() { return C; }

    private:
        float fov    = 45.0f;
        float aspect = 1500.0f / 900.0f;
        float znear  = 0.1f;
        float zfar   = 100.0f;

        float p[3]   = {0.0f, 0.0f, 6.0f};
        float e[3]   = {0.0f, 0.0f, 0.0f};
        float q[4]   = {1.0f, 0.0f, 0.0f, 0.0f};

        float V[16];
        float P[16];
        float C[16];

        friend class Scene;
    };

} // namespace Skynet


#endif // SKYNET_CAMERA_H