#pragma once

#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/variant/basis.hpp>
#include <godot_cpp/variant/quaternion.hpp>

namespace godot {

class Camera : public Camera3D {
	GDCLASS(Camera, Camera3D)

protected:
	static void _bind_methods();

public:
	Camera();
	~Camera();

	const Quaternion &get_camera_quaternion() const;
	const Basis &get_camera_basis() const;
};

} //namespace godot
