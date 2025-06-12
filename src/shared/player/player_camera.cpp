#include "player_camera.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/classes/window.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/math.hpp>

using namespace godot;

void PlayerCamera::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_camera_controller_rotation_speed"), &PlayerCamera::get_camera_controller_rotation_speed);
	ClassDB::bind_method(D_METHOD("set_camera_controller_rotation_speed", "p_controller_rotation_speed"), &PlayerCamera::set_camera_controller_rotation_speed);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "camera_controller_rotation_speed", PROPERTY_HINT_RANGE, "1,20,0.1"), "set_camera_controller_rotation_speed", "get_camera_controller_rotation_speed");

	ClassDB::bind_method(D_METHOD("get_camera_mouse_rotation_speed"), &PlayerCamera::get_camera_mouse_rotation_speed);
	ClassDB::bind_method(D_METHOD("set_camera_mouse_rotation_speed", "p_mouse_rotation_speed"), &PlayerCamera::set_camera_mouse_rotation_speed);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "camera_mouse_rotation_speed", PROPERTY_HINT_RANGE, "0,20,0.001"), "set_camera_mouse_rotation_speed", "get_camera_mouse_rotation_speed");
}

PlayerCamera::PlayerCamera() :
		Camera(),
		input(nullptr),
		camera_controller_rotation_speed(3.0f),
		camera_mouse_rotation_speed(0.001f),
		camera_x_rotation_min(Math::deg_to_rad(-89.9f)),
		camera_x_rotation_max(Math::deg_to_rad(70.0f)) {}

PlayerCamera::~PlayerCamera() {}

void PlayerCamera::_ready() {
	if (Engine::get_singleton()->is_editor_hint()) {
		set_process_input(false);
		set_process(false);
		set_physics_process(false);
		return;
	}

	input = Input::get_singleton();
	ERR_FAIL_NULL_MSG(input, "Input hasn't been initialized yet?");

	input->set_mouse_mode(Input::MOUSE_MODE_CAPTURED);

	camera_base = get_node<Node3D>("../../..");
	ERR_FAIL_NULL_MSG(camera_base, "Failed to find CameraBase node parent");

	camera_rot = get_node<Node3D>("../..");
	ERR_FAIL_NULL_MSG(camera_rot, "Failed to find CameraRot node parent");
}

void PlayerCamera::_input(const Ref<InputEvent> &p_event) {
	// Means debug is active.
	if (input->get_mouse_mode() == Input::MOUSE_MODE_VISIBLE) {
		return;
	}

	const Viewport *viewport = get_viewport();
	const Window *window = viewport->get_window();

	const float scale_factor = Math::min(
			(float(window->get_size().x) / viewport->get_visible_rect().size.x),
			(float(window->get_size().y) / viewport->get_visible_rect().size.y));

	const Ref<InputEventMouseMotion> &mouse_motion_input_event = p_event;
	if (mouse_motion_input_event.is_valid()) {
		rotate_camera(mouse_motion_input_event->get_relative() * camera_mouse_rotation_speed * scale_factor);
	}
}

void PlayerCamera::_process(double p_delta) {
	// Means debug is active.
	if (input->get_mouse_mode() == Input::MOUSE_MODE_VISIBLE) {
		return;
	}

	const Vector2 &camera_motion = input->get_vector("view_left", "view_right", "view_up", "view_down");
	if (!camera_motion.is_zero_approx()) {
		const float camera_speed = p_delta * camera_controller_rotation_speed;
		rotate_camera(camera_motion * camera_speed);
	}
}

void PlayerCamera::set_camera_controller_rotation_speed(const float p_controller_rotation_speed) {
	camera_controller_rotation_speed = p_controller_rotation_speed;
}

float PlayerCamera::get_camera_controller_rotation_speed() const {
	return camera_controller_rotation_speed;
}

void PlayerCamera::set_camera_mouse_rotation_speed(const float p_mouse_rotation_speed) {
	camera_mouse_rotation_speed = p_mouse_rotation_speed;
}

float PlayerCamera::get_camera_mouse_rotation_speed() const {
	return camera_mouse_rotation_speed;
}

Node3D *PlayerCamera::get_camera_base() const {
	return camera_base;
}

Node3D *PlayerCamera::get_camera_rot() const {
	return camera_rot;
}

const Quaternion PlayerCamera::get_camera_quaternion() const {
	return camera_base->get_global_transform().get_basis().get_rotation_quaternion();
}

const Basis &PlayerCamera::get_camera_basis() const {
	return camera_rot->get_global_transform().get_basis();
}

void PlayerCamera::rotate_camera(const Vector2 &p_rotate) {
	camera_base->rotate_y(-p_rotate.x);
	camera_base->orthonormalize();
	const Vector3 &new_rotation = Vector3(Math::clamp(camera_rot->get_rotation().x + p_rotate.y, camera_x_rotation_min, camera_x_rotation_max), camera_rot->get_rotation().y, camera_rot->get_rotation().z);
	camera_rot->set_rotation(new_rotation);
}
