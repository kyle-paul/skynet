#ifndef SKYNET_ENGINE_CAMERA_H
#define SKYNET_ENGINE_CAMERA_H

#include "Titan.hpp"

namespace Skynet
{
    class Camera {
    public:
        Camera();
        ~Camera();

        void UpdateView();
        void UpdateProjection();
        void UpdateProjView();

        inline titan::mat4& GetView() { return V; }
        inline titan::mat4& GetProjection() { return P; }
        inline titan::mat4& GetProjView() { return C; }

    private:
        float fov    = 45.0f;
        float aspect = 1500.0f / 900.0f;
        float znear  = 0.1f;
        float zfar   = 600.0f;

        titan::vec3 p = titan::vec3(0.0f, 0.0f, 6.0f);
        titan::vec3 e;
        titan::quat q;

        titan::mat4 V;
        titan::mat4 P;
        titan::mat4 C;

        friend class Scene;
        friend class Global;
    };

} // namespace Skynet


#endif // SKYNET_ENGINE_CAMERA_H