#include "camera.h"

using namespace godot;

void Camera::_bind_methods() {}

Camera::Camera() :
		Camera3D() {
}

Camera::~Camera() {
}

const Quaternion Camera::get_camera_quaternion() const {
	return get_global_transform().get_basis().get_rotation_quaternion();
}

const Basis &Camera::get_camera_basis() const {
	return get_global_transform().get_basis();
}
