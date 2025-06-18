#include "state_machine.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/class_db.hpp>

#if IMGUI_ENABLED
#include <imgui-godot.h>
#endif

using namespace godot;

void StateMachine::_bind_methods() {
#if IMGUI_ENABLED
	ClassDB::bind_method(D_METHOD("draw_debug"), &StateMachine::draw_debug);
#endif
	ClassDB::bind_method(D_METHOD("get_initial_state"), &StateMachine::get_initial_state);
	ClassDB::bind_method(D_METHOD("set_initial_state", "p_state"), &StateMachine::set_initial_state);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "initial_state", PROPERTY_HINT_RESOURCE_TYPE, "State"), "set_initial_state", "get_initial_state");
}

StateMachine::StateMachine() :
		Node(),
		initial_state(nullptr),
		current_state(nullptr),
		root_node(nullptr) {
}

StateMachine::~StateMachine() {
}

void StateMachine::_ready() {
	if (Engine::get_singleton()->is_editor_hint()) {
		set_process_input(false);
		set_process(false);
		set_physics_process(false);
		return;
	}

	SceneTree *scene_tree = get_tree();
	ERR_FAIL_NULL_MSG(scene_tree, "Failed to get scene tree somehow");
	root_node = scene_tree->get_current_scene();
	ERR_FAIL_NULL_MSG(root_node, "Failed to find root node somehow");

	ERR_FAIL_NULL_MSG(initial_state, "initial_state is nullptr, please make sure this is set to something in the editor.");
	current_state = initial_state;
	current_state->initialize_state(this, root_node);
	current_state->on_enter();
}

void StateMachine::_input(const Ref<InputEvent> &p_event) {
	current_state->input(p_event);
}

void StateMachine::_process(double p_delta) {
	current_state->process(p_delta);

	// Handle any state transitions.
	Ref<State> new_state = current_state->handle_transition();
	if (new_state != nullptr) {
		current_state->on_exit();
		current_state = new_state;
		new_state->on_enter();
	}
}

void StateMachine::_physics_process(double p_delta) {
	current_state->physics_process(p_delta);
}

void StateMachine::set_initial_state(const Ref<State> &p_state) {
	initial_state = p_state;
}

Ref<State> StateMachine::get_initial_state() const {
	return initial_state;
}

Ref<State> StateMachine::get_current_state() const {
	return current_state;
}

void StateMachine::_validate_property(PropertyInfo &p_property) const {
}

void StateMachine::draw_debug() {
#if IMGUI_ENABLED
	ImGui::Text("Current State: %s", current_state->get_name().c_escape().utf8().get_data());
	current_state->draw_debug();
#endif
}
