#pragma once

#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/variant/transform3d.hpp>

#include "character_movement_mode.h"

namespace godot {

class AnimationPlayer;

class CharacterBody : public CharacterBody3D {
	GDCLASS(CharacterBody, CharacterBody3D)

protected:
	static void _bind_methods();

public:
	CharacterBody();
	~CharacterBody();

	void _ready() override;

	void initialize();
	void set_rotation_speed(const float p_rotation_speed);
	float get_rotation_speed() const;

	void set_speed(const float p_speed);
	float get_speed() const;

	void set_movement_mode(const CharacterMovementMode p_mode);
	CharacterMovementMode get_movement_mode() const;

protected:
	AnimationPlayer *animation_player;
	Transform3D orientation;
	Transform3D root_motion;
	Vector3 gravity;
	Vector2 motion;
	float rotation_speed;
	float speed;

	virtual void on_movement_mode_changed();

private:
	CharacterMovementMode movement_mode;
};

} //namespace godot
