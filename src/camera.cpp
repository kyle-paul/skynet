#include "camera.h"
#include "math.h"

Camera::Camera() {
	
}

Camera::~Camera() {

}

void Camera::updateView() {
	math::euler2TF(T, e, p);
}

void Camera::updateProjection() {
	math::perspective(P, aspect, fov, znear, zfar);
}