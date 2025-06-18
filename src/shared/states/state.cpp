#include "state.h"

#include <godot_cpp/classes/node.hpp>

#include "state_machine.h"

using namespace godot;

void State::_bind_methods() {}

State::State() :
		Resource(),
		owner(nullptr),
		root_node(nullptr) {
	set_local_to_scene(true);
}

State::~State() {}

void State::initialize_state(StateMachine *p_owner, Node *p_root_node) {
	ERR_FAIL_NULL_MSG(p_owner, "p_owner is null");
	owner = p_owner;

	ERR_FAIL_NULL_MSG(p_root_node, "p_root_node is null");
	root_node = p_root_node;

	initialize();
}

void State::initialize() {}

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
