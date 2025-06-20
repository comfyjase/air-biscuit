#pragma once

#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/variant/vector2.hpp>

namespace godot {

class Input;
class Player;
class StateMachine;
class VirtualJoystick;

class PlayerInput : public Node {
	GDCLASS(PlayerInput, Node)

protected:
	static void _bind_methods();

public:
	PlayerInput();
	~PlayerInput();

	void _ready() override;
	void _input(const Ref<InputEvent> &p_event) override;
	void _process(double delta) override;

	const Vector2 &get_motion() const;

private:
	Player *player;
	StateMachine *state_machine;
	VirtualJoystick *virtual_left_joystick;
	Input *input;
	Vector2 motion;
};

} //namespace godot
