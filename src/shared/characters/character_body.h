#pragma once

#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/variant/transform3d.hpp>

#include "character_movement_mode.h"

namespace godot {

class CharacterBody : public CharacterBody3D {
	GDCLASS(CharacterBody, CharacterBody3D)

protected:
	static void _bind_methods();

public:
	CharacterBody();
	~CharacterBody();

	void set_speed(const float p_speed);
	float get_speed() const;

protected:
	float speed;
};

} //namespace godot
