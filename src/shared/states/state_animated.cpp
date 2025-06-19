#include "state_animated.h"

#include <godot_cpp/classes/animation_player.hpp>
#include <godot_cpp/classes/node.hpp>

#include "function_library.h"
#include "state_machine.h"

using namespace godot;

void StateAnimated::_bind_methods() {}

StateAnimated::StateAnimated() :
		State(),
		animation_player(nullptr),
		current_animation_name("") {
}

void StateAnimated::initialize_animation() {
	animation_player = FunctionLibrary::get_singleton()->find_node<AnimationPlayer>(owner->get_parent());
}

void StateAnimated::initialize() {
	initialize_animation();
}

void StateAnimated::animate(String p_name, double p_delta) {
	ERR_FAIL_NULL_MSG(animation_player, "StateAnimated::animation_player is null, has initialize_animation been called?");
	current_animation_name = p_name;
	animation_player->play(p_name);
}

void godot::StateAnimated::animate_and_queue_idle(String p_name, double p_delta) {
	animate(p_name, p_delta);
	animation_player->queue("Idle");
}

void StateAnimated::set_animation_player(AnimationPlayer *p_animation_player) {
	animation_player = p_animation_player;
}

AnimationPlayer *StateAnimated::get_animation_player() const {
	return animation_player;
}

bool StateAnimated::has_animation_finished() const {
	return (animation_player->is_playing() && (animation_player->get_current_animation() == "Idle"));
}
