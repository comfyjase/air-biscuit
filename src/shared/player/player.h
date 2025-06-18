#pragma once

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
	Player();
	~Player();

	void _ready() override;
	void _physics_process(double p_delta) override;

private:
	PlayerCamera *camera;
	PlayerInput *input;
	StateMachine *state_machine;

	void draw_debug();
};

} //namespace godot
