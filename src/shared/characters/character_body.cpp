#include "character_body.h"

#include <godot_cpp/classes/animation_player.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/type_info.hpp>

using namespace godot;

void CharacterBody::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_rotation_speed", "p_speed"), &CharacterBody::set_rotation_speed);
	ClassDB::bind_method(D_METHOD("get_rotation_speed"), &CharacterBody::get_rotation_speed);
	ClassDB::bind_method(D_METHOD("set_speed", "p_speed"), &CharacterBody::set_speed);
	ClassDB::bind_method(D_METHOD("get_speed"), &CharacterBody::get_speed);
	ClassDB::bind_method(D_METHOD("set_movement_mode", "p_mode"), &CharacterBody::set_movement_mode);
	ClassDB::bind_method(D_METHOD("get_movement_mode"), &CharacterBody::get_movement_mode);

	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "rotation_speed", PROPERTY_HINT_RANGE, "1,20,0.01"), "set_rotation_speed", "get_rotation_speed");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "speed", PROPERTY_HINT_RANGE, "0,20,0.01"), "set_speed", "get_speed");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "movement_mode", PROPERTY_HINT_ENUM, "MOVEMENT_IDLE,MOVEMENT_WALKING"), "set_movement_mode", "get_movement_mode");

	BIND_ENUM_CONSTANT(MOVEMENT_IDLE);
	BIND_ENUM_CONSTANT(MOVEMENT_WALKING);
}

CharacterBody::CharacterBody() :
		CharacterBody3D(),
		animation_player(nullptr),
		orientation(),
		root_motion(),
		gravity((float)ProjectSettings::get_singleton()->get_setting("physics/3d/default_gravity") * (Vector3)ProjectSettings::get_singleton()->get_setting("physics/3d/default_gravity_vector")),
		motion(),
		rotation_speed(10.0f),
		speed(5.0f),
		movement_mode(MOVEMENT_IDLE) {
}

CharacterBody::~CharacterBody() {
}

void CharacterBody::_ready() {
	if (Engine::get_singleton()->is_editor_hint()) {
		set_process_input(false);
		set_process(false);
		set_physics_process(false);
		return;
	}

	initialize();
}

void CharacterBody::initialize() {
	orientation = get_global_transform();
	orientation.origin = Vector3();
}

void godot::CharacterBody::set_rotation_speed(const float p_rotation_speed) {
	rotation_speed = p_rotation_speed;
}

float godot::CharacterBody::get_rotation_speed() const {
	return rotation_speed;
}

void CharacterBody::set_speed(const float p_speed) {
	speed = p_speed;
}

float CharacterBody::get_speed() const {
	return speed;
}

void CharacterBody::set_movement_mode(const CharacterMovementMode p_mode) {
	if (movement_mode != p_mode) {
		movement_mode = p_mode;
		on_movement_mode_changed();
	}
}

CharacterMovementMode CharacterBody::get_movement_mode() const {
	return movement_mode;
}

void CharacterBody::on_movement_mode_changed() {}
