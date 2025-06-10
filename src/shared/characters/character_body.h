#pragma once

#include <godot_cpp/classes/character_body3d.hpp>

namespace godot {

class CharacterBody : public CharacterBody3D {
	GDCLASS(CharacterBody, CharacterBody3D)

protected:
	static void _bind_methods();

public:
	CharacterBody();
	~CharacterBody();
};

} //namespace godot
