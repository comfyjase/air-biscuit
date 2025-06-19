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
	void animate_and_queue_idle(String p_name, double p_delta);

	void set_animation_player(AnimationPlayer* p_animation_player);
	AnimationPlayer* get_animation_player() const;

	bool has_animation_finished() const;

protected:
	AnimationPlayer* animation_player;
	String current_animation_name;
};

} //namespace godot
