#include "character_body.h"

#include <godot_cpp/classes/animation_player.hpp>
#include <godot_cpp/classes/animation_tree.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/type_info.hpp>

using namespace godot;

void CharacterBody::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_speed", "p_speed"), &CharacterBody::set_speed);
	ClassDB::bind_method(D_METHOD("get_speed"), &CharacterBody::get_speed);

	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "speed", PROPERTY_HINT_RANGE, "0,20,0.01"), "set_speed", "get_speed");
}

CharacterBody::CharacterBody() :
		CharacterBody3D(),
		speed(5.0f) {
}

CharacterBody::~CharacterBody() {
}

void CharacterBody::set_speed(const float p_speed) {
	speed = p_speed;
}

float CharacterBody::get_speed() const {
	return speed;
}
