#include "Camera.h"

namespace Skynet
{
    Camera::Camera() {
        
    }

    Camera::~Camera() {
        
    }

    void Camera::UpdateView() 
    {
        this->V = titan::Translate(p) * titan::Euler2T(e);
    }

    void Camera::UpdateProjection() 
    {
        this->P = titan::Perspective(fov, aspect, znear, zfar);
    }

    void Camera::UpdateProjView() 
    {
        this->UpdateProjection();
        this->UpdateView();
        this->C = P * V;
    }

} // namespace Skynet
