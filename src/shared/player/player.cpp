#include "player.h"

#include <godot_cpp/classes/animation_player.hpp>
#include <godot_cpp/classes/animation_tree.hpp>
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
		input(nullptr),
		current_animation_state(ANIMATIONS::IDLE) {
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

	camera = get_node<PlayerCamera>("CameraBase/CameraRot/SpringArm3D/Camera");
	ERR_FAIL_NULL_MSG(camera, "Failed to get player camera child node");

	input = get_node<PlayerInput>("Input");
	ERR_FAIL_NULL_MSG(input, "Failed to get player input child node");

	animation_player = get_node<AnimationPlayer>("Shibby/AnimationPlayer");
	ERR_FAIL_NULL_MSG(animation_player, "Failed to get animation player");

	model = get_node<Node3D>("Shibby");
	ERR_FAIL_NULL_MSG(animation_player, "Failed to get player model");

	initialize();

	//animation_tree = get_node<AnimationTree>("AnimationTree");
	//ERR_FAIL_NULL_MSG(animation_tree, "Failed to get animation tree");
}

void Player::_physics_process(double p_delta) {
	process_input(p_delta);
}

void Player::animate(int p_animation, double p_delta) {
	current_animation_state = (ANIMATIONS)p_animation;

	switch (current_animation_state) {
		case ANIMATIONS::IDLE: {
			animation_player->play("Idle");
			break;
		}
		case ANIMATIONS::WALK: {
			animation_player->play("Walk");
			break;
		}
		default: {
			ERR_FAIL_COND_MSG(true, "Unknown player animation requested to play %d" + String::num_int64(p_animation));
			break;
		}
	}
}

void Player::process_input(double p_delta) {
	if (input->get_motion().is_zero_approx()) {
		animate(ANIMATIONS::IDLE, p_delta);
		return;
	}

	Vector3 new_velocity = get_velocity();

	//if (!is_on_floor()) {
	//	new_velocity += get_gravity() * p_delta;
	//}

	//if (input->is_jumping() && is_on_floor()) {
	//	new_velocity.y += jump_speed;
	//}

	const float horizontal_rotation = camera->get_camera_basis().get_euler().y;

	const Vector3 &direction = get_transform().get_basis().xform(Vector3(input->get_motion().x, 0.0f, input->get_motion().y)).rotated(Vector3(0.0f, 1.0f, 0.0f), horizontal_rotation).normalized();
	if (!direction.is_zero_approx()) {
		new_velocity.x = direction.x * speed;
		new_velocity.z = direction.z * speed;
	} else {
		new_velocity.x = Math::move_toward(get_velocity().x, 0.0f, speed);
		new_velocity.z = Math::move_toward(get_velocity().z, 0.0f, speed);
	}

	animate(ANIMATIONS::WALK, p_delta);

	set_velocity(new_velocity);
	move_and_slide();

	Vector3 look_at_position = get_global_position() + direction;
	look_at_position.y = get_global_position().y;
	model->look_at(look_at_position);
}

void Player::draw_debug() {
#if IMGUI_ENABLED

	ImGui::Text("State");
	String current_animation_state_as_string = "Unknown";
	switch (current_animation_state) {
		case ANIMATIONS::IDLE: {
			current_animation_state_as_string = "Idle";
			break;
		}
		case ANIMATIONS::WALK: {
			current_animation_state_as_string = "Walk";
			break;
		}
		default: {
			ERR_FAIL_COND_MSG(true, "Unknown player animation requested to play %d" + String::num_int64(current_animation_state));
			break;
		}
	}
	ImGui::Text(current_animation_state_as_string.c_escape().utf8().get_data());

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
