#include "player_input.h"

#include <godot_cpp/classes/input.hpp>

using namespace godot;

void PlayerInput::_bind_methods() {}

PlayerInput::PlayerInput() :
		Node(),
		input(nullptr),
		motion() {
}

PlayerInput::~PlayerInput() {
}

void PlayerInput::_ready() {
	input = Input::get_singleton();
	ERR_FAIL_NULL_MSG(input, "Input hasn't been initialized yet?");
}

void PlayerInput::_input(const Ref<InputEvent> &p_event) {
}

void PlayerInput::_process(double delta) {
	motion = Vector2(input->get_action_strength("move_right") - input->get_action_strength("move_left"),
			input->get_action_strength("move_back") - input->get_action_strength("move_forward"));
}

const Vector2& PlayerInput::get_motion() const {
	return motion;
}
