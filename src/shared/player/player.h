#pragma once

#include <godot_cpp/variant/vector2.hpp>

#include "character_body.h"

namespace godot {

class PlayerCamera;
class PlayerInput;
class StateMachine;

class Player : public CharacterBody {
	GDCLASS(Player, CharacterBody)

protected:
	static void _bind_methods();

public:
	enum ANIMATIONS {
		ATTACK,
		IDLE,
		WALK
	};

	Player();
	~Player();

	void _ready() override;
	void _physics_process(double p_delta) override;

	void animate(int p_animation, double p_delta);

private:
	PlayerCamera *camera;
	PlayerInput *input;
	StateMachine *state_machine;
	ANIMATIONS current_animation_state;

	void process_input(double p_delta);
	void draw_debug();
};

} //namespace godot
