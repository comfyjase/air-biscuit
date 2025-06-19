#include "state_attack.h"

#include <godot_cpp/classes/animation_player.hpp>

using namespace godot;

void StateAttack::_bind_methods() {}

StateAttack::StateAttack() :
		StateAnimated() {}

StateAttack::~StateAttack() {}

void StateAttack::on_enter() {
	animate_and_queue_idle("Attack", 0.0);
}

const String StateAttack::get_name() const {
	return "Attack";
}
