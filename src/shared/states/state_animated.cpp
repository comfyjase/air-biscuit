#include "state_animated.h"

#include <godot_cpp/classes/animation_player.hpp>
#include <godot_cpp/classes/node.hpp>

#include "function_library.h"
#include "state_machine.h"

using namespace godot;

void StateAnimated::_bind_methods() {}

StateAnimated::StateAnimated() :
		State(),
		animation_player(nullptr) {
}

void StateAnimated::initialize_animation() {
	animation_player = FunctionLibrary::get_singleton()->find_node<AnimationPlayer>(owner->get_parent());
}

void StateAnimated::initialize() {
	initialize_animation();
}

void StateAnimated::animate(String p_name, double p_delta) {
	ERR_FAIL_NULL_MSG(animation_player, "StateAnimated::animation_player is null, has initialize_animation been called?");
	animation_player->play(p_name);
}

void StateAnimated::set_animation_player(AnimationPlayer *p_animation_player) {
	animation_player = p_animation_player;
}

AnimationPlayer *StateAnimated::get_animation_player() const {
	return animation_player;
}
