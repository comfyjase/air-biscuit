#pragma once

#include "state.h"

namespace godot {

	class AnimationPlayer;

class StateAnimated : public State {
	GDCLASS(StateAnimated, State);

protected:
	static void _bind_methods();

public:
	StateAnimated();

	void initialize_animation();

	void initialize() override;

	void animate(String p_name, double p_delta);

	void set_animation_player(AnimationPlayer* p_animation_player);
	AnimationPlayer* get_animation_player() const;

private:
	AnimationPlayer* animation_player;
};

} //namespace godot
