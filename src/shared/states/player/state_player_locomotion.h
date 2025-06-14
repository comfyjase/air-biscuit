#pragma once

#include "state.h"

namespace godot {

class StatePlayerLocomotion : public State {
	GDCLASS(StatePlayerLocomotion, State);

protected:
	static void _bind_methods();

public:
	StatePlayerLocomotion();
	~StatePlayerLocomotion();

	const String get_name() const override;

	void draw_debug() override;
};

} //namespace godot
