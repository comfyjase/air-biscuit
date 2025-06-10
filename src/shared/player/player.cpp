#include "player.h"

#include <godot_cpp/variant/vector3.hpp>

#if IMGUI_ENABLED
#include <imgui-godot.h>
#endif

#include "player_input.h"

using namespace godot;

void Player::_bind_methods() {
#if IMGUI_ENABLED
	ClassDB::bind_method(D_METHOD("draw_debug"), &Player::draw_debug);
#endif
}

Player::Player() :
		CharacterBody(),
		input(nullptr),
		motion() {
}

Player::~Player() {
}

void Player::_ready() {
	input = memnew(PlayerInput);
	add_child(input);
}

void Player::_physics_process(double p_delta) {
	apply_input(p_delta);
}

void Player::apply_input(double p_delta) {
	// TEMP: Do player states for locomotion or something.
	// Implement generic state machine pattern.
	// Have locomotion states: idle, walking...
	motion = motion.lerp(input->get_motion(), 10.0 * p_delta);

	if (!motion.is_zero_approx()) {
		Vector3 velocity = get_velocity();
		velocity.x += motion.x;
		velocity.z += motion.y;

		set_velocity(velocity);
		set_up_direction(Vector3(0.0, 1.0, 0.0));
		move_and_slide();
	}
}

void Player::draw_debug() {
#if IMGUI_ENABLED
	ImGui::Text("Input Motion");

	float motion_x = motion.x;
	float motion_y = motion.y;

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
