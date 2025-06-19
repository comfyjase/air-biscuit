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
	ClassDB::bind_method(D_METHOD("get_states"), &StateMachine::get_states);
	ClassDB::bind_method(D_METHOD("set_states", "p_states"), &StateMachine::set_states);

	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "states",
					PROPERTY_HINT_TYPE_STRING,
					String::num(Variant::OBJECT) + "/" + String::num(PROPERTY_HINT_RESOURCE_TYPE) + "State"), "set_states", "get_states");
}

StateMachine::StateMachine() :
		Node(),
		states(),
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

	ERR_FAIL_COND_MSG(states.size() == 0, "states is empty, please fill them with something in the editor.");
	current_state = states[0];
	current_state->initialize_state(this, root_node);
	current_state->on_enter();
}

void StateMachine::_input(const Ref<InputEvent> &p_event) {
	current_state->input(p_event);
}

void StateMachine::_process(double p_delta) {
	current_state->process(p_delta);
}

void StateMachine::_physics_process(double p_delta) {
	current_state->physics_process(p_delta);
}

void StateMachine::transition(String new_state_name) {
	Ref<State> new_state = get_state(new_state_name);

	if (new_state.is_valid()) {
		// Exit the current state.
		current_state->on_exit();

		// Change to the new state.
		current_state = new_state;

		// Initialize and enter the new state.
		new_state->initialize_state(this, root_node);
		new_state->on_enter();
	}
}

void StateMachine::set_states(const TypedArray<Ref<State>> p_states) {
	states = p_states;
}

TypedArray<Ref<State>> StateMachine::get_states() const {
	return states;
}

Ref<State> StateMachine::get_current_state() const {
	return current_state;
}

void StateMachine::_validate_property(PropertyInfo &p_property) const {
	if (p_property.name.c_unescape() == "states") {
		// Validation checks for empty state machine.
		if (states.size() == 0) {
			WARN_PRINT("The states array should have at least one state.");
		} else {
			// Validation checks for null states.
			for (int i = 0; i < states.size(); ++i) {
				Ref<State> state = states[i];
				if (state.is_null() || !state.is_valid()) {
					WARN_PRINT("State " + String::num_int64(i) + " is invalid. Please assign a state or remove element this from the array.");
				}
			}
		}
	}
}

Ref<State> godot::StateMachine::get_state(const String &p_name) const {
	Ref<State> state = nullptr;

	for (int i = 0; i < states.size(); ++i) {
		Ref<State> temp_state = states[i];
		if (temp_state.is_valid() && temp_state->get_name() == p_name) {
			state = temp_state;
			break;
		}
	}

	return state;
}

void StateMachine::draw_debug() {
#if IMGUI_ENABLED
	ImGui::Text("Current State: %s", current_state->get_name().c_escape().utf8().get_data());
	current_state->draw_debug();
#endif
}
