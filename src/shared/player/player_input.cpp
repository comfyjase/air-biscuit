#include "player_input.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/core/class_db.hpp>

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
	if (Engine::get_singleton()->is_editor_hint()) {
		set_process_input(false);
		set_process(false);
		set_physics_process(false);
		return;
	}

	input = Input::get_singleton();
	ERR_FAIL_NULL_MSG(input, "Input hasn't been initialized yet?");
}

void PlayerInput::_input(const Ref<InputEvent> &p_event) {
}

void PlayerInput::_process(double delta) {
	motion = input->get_vector("move_left", "move_right", "move_forward", "move_back");
}

const Vector2 &PlayerInput::get_motion() const {
	return motion;
}
