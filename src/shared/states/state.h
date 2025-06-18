#pragma once

#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/string.hpp>

namespace godot {

class Node;
class StateMachine;

class State : public Resource {
	GDCLASS(State, Resource);

protected:
	static void _bind_methods();

public:
	State();
	~State();

	void initialize_state(StateMachine *p_owner, Node *p_root_node);

	virtual void initialize();
	virtual void on_enter();
	virtual void input(const Ref<InputEvent> &p_event);
	virtual void process(double p_delta);
	virtual void physics_process(double p_delta);
	virtual Ref<State> handle_transition();
	virtual void on_exit();

	virtual const String get_name() const;

	virtual void draw_debug();

protected:
	StateMachine *owner;
	Node *root_node;
};

} //namespace godot
