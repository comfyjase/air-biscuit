#include "player_input.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/core/class_db.hpp>

#include "function_library.h"
#include "player.h"
#include "state_machine.h"
#include "state_attack.h"

using namespace godot;

void PlayerInput::_bind_methods() {}

PlayerInput::PlayerInput() :
		Node(),
		player(nullptr),
		input(nullptr),
		motion() {
}

PlayerInput::~PlayerInput() {
}

void PlayerInput::_ready() {
	if (Engine::get_singleton()->is_editor_hint()) {
		set_process_input(false);
		set_process(false);
		set_physics_process(false);
		return;
	}

	input = Input::get_singleton();
	ERR_FAIL_NULL_MSG(input, "Input hasn't been initialized yet?");

	player = Object::cast_to<Player>(get_parent());
	ERR_FAIL_NULL_MSG(player, "player is nullptr, is this Input node a child of a Player class node?");

	state_machine = FunctionLibrary::get_singleton()->find_node<StateMachine>(player);
	ERR_FAIL_NULL_MSG(state_machine, "state_machine is nullptr, does the player scene have a StateMachine node attached?");
}

void PlayerInput::_input(const Ref<InputEvent> &p_event) {
	if (input->is_action_just_pressed("attack")) {
		state_machine->transition("Attack");
	}
}

void PlayerInput::_process(double delta) {
	motion = input->get_vector("move_right", "move_left", "move_back", "move_forward");

	Ref<StateAttack> attack_state = state_machine->get_current_state();
	if (attack_state.is_valid()) {
		if (attack_state->has_animation_finished()) {
			state_machine->transition("Locomotion");
		}
	}
}

const Vector2 &PlayerInput::get_motion() const {
	return motion;
}
