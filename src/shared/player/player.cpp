#include "player.h"

#include <godot_cpp/classes/animation_player.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/vector3.hpp>

#if IMGUI_ENABLED
#include <imgui-godot.h>
#endif

#include "player_camera.h"
#include "player_input.h"

using namespace godot;

void Player::_bind_methods() {
#if IMGUI_ENABLED
	ClassDB::bind_method(D_METHOD("draw_debug"), &Player::draw_debug);
#endif
}

Player::Player() :
		CharacterBody(),
		camera(nullptr),
		input(nullptr) {
}

Player::~Player() {
}

void Player::_ready() {
	if (Engine::get_singleton()->is_editor_hint()) {
		set_process_input(false);
		set_process(false);
		set_physics_process(false);
		return;
	}

	initialize();

	camera = get_node<PlayerCamera>("Camera");
	ERR_FAIL_NULL_MSG(camera, "Failed to get player camera child node");

	input = get_node<PlayerInput>("Input");
	ERR_FAIL_NULL_MSG(input, "Failed to get player input child node");

	animation_player = get_node<AnimationPlayer>("Shibby/AnimationPlayer");
	ERR_FAIL_NULL_MSG(animation_player, "Failed to get animation player");
}

void Player::_physics_process(double p_delta) {
	process_input(p_delta);
}

void Player::process_input(double p_delta) {
	Vector3 new_velocity = get_velocity();

	//if (!is_on_floor()) {
	//	new_velocity += get_gravity() * p_delta;
	//}

	//if (input->is_jumping() && is_on_floor()) {
	//	new_velocity.y += jump_speed;
	//}

	const float horizontal_rotation = camera->get_basis().get_euler().y;

	const Vector3 &direction = get_transform().get_basis().xform(Vector3(input->get_motion().x, 0.0f, input->get_motion().y)).rotated(Vector3(0.0f, 1.0f, 0.0f), horizontal_rotation).normalized();
	if (!direction.is_zero_approx()) {
		new_velocity.x = direction.x * speed;
		new_velocity.z = direction.z * speed;

		//look_at();
	} else {
		new_velocity.x = Math::move_toward(get_velocity().x, 0.0f, speed);
		new_velocity.z = Math::move_toward(get_velocity().z, 0.0f, speed);
	}

	set_velocity(new_velocity);
	move_and_slide();
}

void Player::draw_debug() {
#if IMGUI_ENABLED
	ImGui::Text("New stuff is being added");

	ImGui::Text("Input Motion");

	float motion_x = input->get_motion().x;
	float motion_y = input->get_motion().y;

	ImGui::DragFloat("X: ##motion", &motion_x);
	ImGui::DragFloat("Y: ##motion", &motion_y);

	ImGui::Separator();

	ImGui::Text("Velocity");

	float velocity_x = get_velocity().x;
	float velocity_y = get_velocity().y;
	float velocity_z = get_velocity().z;

	ImGui::DragFloat("X: ##velocity", &velocity_x);
	ImGui::DragFloat("Y: ##velocity", &velocity_y);
	ImGui::DragFloat("Z: ##velocity", &velocity_z);
#endif
}
