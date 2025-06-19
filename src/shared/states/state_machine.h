#pragma once

#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/core/property_info.hpp>
#include <godot_cpp/core/type_info.hpp>
#include <godot_cpp/variant/typed_array.hpp>

#include "state.h"

namespace godot {

MAKE_TYPED_ARRAY_INFO(Ref<State>, Variant::OBJECT);
MAKE_TYPED_ARRAY(Ref<State>, Variant::OBJECT);

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

	void transition(String new_state_name);

	void set_states(const TypedArray<Ref<State>> p_states);
	TypedArray<Ref<State>> get_states() const;

	Ref<State> get_current_state() const;

protected:
	void _validate_property(PropertyInfo &p_property) const;

private:
	TypedArray<Ref<State>> states;
	Ref<State> current_state;

	Node *root_node;

	Ref<State> get_state(const String &p_name) const;

	void draw_debug();
};

} //namespace godot
