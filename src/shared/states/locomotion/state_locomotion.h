#pragma once

#include "state_animated.h"

#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/variant/vector2.hpp>

namespace godot {

	class CharacterBody3D;
	class Node3D;

class StateLocomotion : public StateAnimated {
	GDCLASS(StateLocomotion, StateAnimated);

protected:
	static void _bind_methods();

public:
	StateLocomotion();
	~StateLocomotion();

	void on_enter() override;
	void physics_process(double p_delta) override;
	void on_exit() override;

	const String get_name() const override;
	void draw_debug() override;
	

	void set_node_to_update_look_at(Node3D *p_node);
	void set_input_velocity(const Vector2 &p_input_velocity);
	void set_rotation(const float p_rotation);
	void set_speed(const float p_speed);

private:
	CharacterBody3D *character_body;
	Node3D *node_to_update_look_at;
	Vector2 input_velocity;
	float speed;
	float rotation;
};

} //namespace godot
