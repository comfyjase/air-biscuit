#pragma once

#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/core/property_info.hpp>
#include <godot_cpp/variant/array.hpp>

#include "state.h"

namespace godot {

class StateMachine : public Node {
	GDCLASS(StateMachine, Node);

protected:
	static void _bind_methods();

public:
	StateMachine();
	~StateMachine();

	void _ready() override;
	void _input(const Ref<InputEvent> &p_event) override;
	void _process(double p_delta) override;
	void _physics_process(double p_delta) override;

	void set_initial_state(const Ref<State>& p_state);
	Ref<State> get_initial_state() const;
	Ref<State> get_current_state() const;

protected:
	void _validate_property(PropertyInfo &p_property) const;

private:
	Ref<State> initial_state;
	Ref<State> current_state;

	Node *root_node;

	void draw_debug();
};

} //namespace godot
