#pragma once

#include <godot_cpp/classes/camera3d.hpp>

namespace godot {

class Camera : public Camera3D {
	GDCLASS(Camera, Camera3D)

protected:
	static void _bind_methods();

public:
	Camera();
	~Camera();
};

} //namespace godot
