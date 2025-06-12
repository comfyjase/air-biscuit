#pragma once

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/type_info.hpp>

namespace godot {

enum CharacterMovementMode {
	MOVEMENT_IDLE,
	MOVEMENT_WALKING
};

} //namespace godot

VARIANT_ENUM_CAST(CharacterMovementMode);
