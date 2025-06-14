#include "state.h"

using namespace godot;

void State::_bind_methods() {}

State::State() {}

State::~State() {}

void State::on_enter() {}

void State::input(const Ref<InputEvent> &p_event) {}

void State::process(double p_delta) {}

void State::physics_process(double p_delta) {}

Ref<State> State::handle_transition() {
	return nullptr;
}

void State::on_exit() {}

const String State::get_name() const {
	return "State";
}

void State::draw_debug() {}
