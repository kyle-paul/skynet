#include "camera.h"
#include "math.h"

    
Camera::Camera() {
	
}

Camera::~Camera() {
	
}

void Camera::updateView() {
	math::euler2TF(V, e, p);
}


void Camera::updateProjection() {
	math::perspective(P, fov, aspect, znear, zfar);
}

void Camera::updateProjView() {
	this->updateProjection();
	this->updateView();
	math::matmul4(C, P, V);
}