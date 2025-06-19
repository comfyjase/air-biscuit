#pragma once

#include "state_animated.h"

namespace godot {

class StateAttack : public StateAnimated {
	GDCLASS(StateAttack, StateAnimated);

protected:
	static void _bind_methods();

public:
	StateAttack();
	~StateAttack();

	void on_enter() override;

	const String get_name() const override;
};

} //namespace godot
