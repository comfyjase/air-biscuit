#pragma once

#include "state.h"

namespace godot {

class StateLocomotion : public State {
	GDCLASS(StateLocomotion, State);

protected:
	static void _bind_methods();

public:
	StateLocomotion();
	~StateLocomotion();

	const String get_name() const override;

	void draw_debug() override;
};

} //namespace godot
