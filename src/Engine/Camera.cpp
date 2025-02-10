#include "Camera.h"

namespace Skynet
{
    Camera::Camera() {
        
    }

    Camera::~Camera() {
        
    }

    void Camera::UpdateView() 
    {
        Math::Euler2T(V, e, p);
    }

    void Camera::UpdateProjection() 
    {
        Math::Perspective(P, fov, aspect, znear, zfar);
    }

    void Camera::UpdateProjView() 
    {
        this->UpdateProjection();
        this->UpdateView();

        Math::Matmul4(C, P, V);
    }

} // namespace Skynet
