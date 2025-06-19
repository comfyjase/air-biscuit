#include "player.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/vector3.hpp>

#if IMGUI_ENABLED
#include <imgui-godot.h>
#endif

#include "player_camera.h"
#include "player_input.h"
#include "state_locomotion.h"
#include "state_machine.h"

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
		state_machine(nullptr) {
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

	state_machine = get_node<StateMachine>("StateMachine");
	ERR_FAIL_NULL_MSG(state_machine, "Failed to get player state machine");

	Ref<StateLocomotion> locomotion = state_machine->get_current_state();
	if (locomotion.is_valid()) {
		Node3D *model = get_node<Node3D>("Shibby");
		locomotion->set_node_to_update_look_at(model);
		locomotion->set_speed(speed);
	}
}

void Player::_physics_process(double p_delta) {
	Ref<StateLocomotion> locomotion = state_machine->get_current_state();
	if (locomotion.is_valid()) {
		locomotion->set_input_velocity(input->get_motion());
		locomotion->set_rotation(camera->get_camera_basis().get_euler().y);
	}
}

void Player::draw_debug() {
#if IMGUI_ENABLED

#endif
}
