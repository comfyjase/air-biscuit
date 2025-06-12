#pragma once

#include "camera.h"

#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/variant/quaternion.hpp>

namespace godot {

class Input;

class PlayerCamera : public Camera {
	GDCLASS(PlayerCamera, Camera)

protected:
	static void _bind_methods();

public:
	PlayerCamera();
	~PlayerCamera();

	void _ready() override;
	void _input(const Ref<InputEvent> &p_event) override;
	void _process(double p_delta) override;

	void set_camera_controller_rotation_speed(const float p_controller_rotation_speed);
	float get_camera_controller_rotation_speed() const;

	void set_camera_mouse_rotation_speed(const float p_mouse_rotation_speed);
	float get_camera_mouse_rotation_speed() const;

	Node3D *get_camera_base() const;
	Node3D *get_camera_rot() const;

	const Quaternion get_camera_quaternion() const override;
	const Basis &get_camera_basis() const override;

private:
	Node3D *camera_base;
	Node3D *camera_rot;
	Input *input;
	float camera_controller_rotation_speed;
	float camera_mouse_rotation_speed;

	float camera_x_rotation_min;
	float camera_x_rotation_max;

	void rotate_camera(const Vector2 &p_rotate);
};

} //namespace godot
