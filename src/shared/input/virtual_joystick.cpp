#include "virtual_joystick.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/image_texture.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event_screen_drag.hpp>
#include <godot_cpp/classes/input_event_screen_touch.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/print_string.hpp>

using namespace godot;

void VirtualJoystick::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_padding"), &VirtualJoystick::get_padding);
	ClassDB::bind_method(D_METHOD("set_padding", "p_padding"), &VirtualJoystick::set_padding);

	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "padding"), "set_padding", "get_padding");
}

VirtualJoystick::VirtualJoystick() :
		TextureRect(),
		joystick(nullptr),
		input(nullptr),
		padding(),
		touch_starting_position(),
		touch_current_position(),
		joystick_initial_position(),
		motion(),
		centre(),
		min(),
		max(),
		touch_area_size(200.0f),
		touch_area_size_squared(touch_area_size * touch_area_size),
		touch_index(-1),
		is_touch_over_joystick_area(false),
		swiping(false),
		resetting(false) {}

VirtualJoystick::~VirtualJoystick() {}

void VirtualJoystick::_ready() {
	if (Engine::get_singleton()->is_editor_hint()) {
		set_process_input(false);
		set_process(false);
		set_physics_process(false);
		return;
	}

#if PLATFORM_ANDROID || PLATFORM_IOS
	input = Input::get_singleton();
	ERR_FAIL_NULL_MSG(input, "Input hasn't been initialized yet?");

	set_position(Vector2(get_position().x + padding.x, get_position().y + padding.y));

	const Ref<Texture2D> &joystick_background_texture = ResourceLoader::get_singleton()->load("res://textures/virtual_joystick_background.png");
	const Ref<Texture2D> &joystick_texture = ResourceLoader::get_singleton()->load("res://textures/virtual_joystick.png");

	set_texture(joystick_background_texture);

	centre = Vector2(get_global_position().x + touch_area_size, get_global_position().y + touch_area_size);

	joystick = memnew(TextureRect);
	joystick->set_name("Joystick");
	joystick->set_texture(joystick_texture);
	add_child(joystick);

	joystick_initial_position = joystick->get_position();

	min = Vector2(joystick_initial_position.x - touch_area_size, joystick_initial_position.y - touch_area_size);
	max = Vector2(joystick_initial_position.x + touch_area_size, joystick_initial_position.y + touch_area_size);
#else
	set_process_input(false);
	set_process(false);
	set_physics_process(false);
#endif
}

void VirtualJoystick::_input(const Ref<InputEvent> &p_event) {
	// First touch on the screen.
	const Ref<InputEventScreenTouch> &input_touch_event = p_event;
	if (input_touch_event.is_valid()) {
		if (input_touch_event->is_pressed()) {
			const Vector2 &touch_position = input_touch_event->get_position();

			if (((touch_position - centre).length_squared() < touch_area_size_squared) && touch_index == -1) {
				if (!is_touch_over_joystick_area) {
					is_touch_over_joystick_area = true;
				}

				touch_starting_position = touch_position;
				touch_index = input_touch_event->get_index();
				get_viewport()->set_input_as_handled();
			}
		} else if (input_touch_event->get_index() == touch_index) {
			is_touch_over_joystick_area = false;
			resetting = true;
		}
	} else {
		// Continous drag on the screen.
		const Ref<InputEventScreenDrag> &input_drag_event = p_event;
		if (input_drag_event.is_valid()) {
			if (input_drag_event->get_index() == touch_index) {
				const Vector2 &touch_position = input_drag_event->get_position();

				if ((touch_position - centre).length_squared() < touch_area_size_squared) {
					touch_current_position = touch_position;
					get_viewport()->set_input_as_handled();
				} else if (input_drag_event->get_index() == touch_index) {
					is_touch_over_joystick_area = false;
					resetting = true;
				}
			}
		}
	}
}

void VirtualJoystick::_process(double p_delta) {
	if (resetting) {
		joystick->set_position(joystick_initial_position);
		motion = Vector2();
		touch_starting_position = Vector2();
		touch_current_position = Vector2();
		touch_index = -1;
		resetting = false;
		return;
	}

	if (!is_touch_over_joystick_area) {
		return;
	}

	motion = (touch_current_position - touch_starting_position);

	Vector2 new_virtual_joystick_position = joystick_initial_position + motion;
	joystick->set_position(new_virtual_joystick_position.clamp(min, max));
}

void VirtualJoystick::set_padding(const Vector2 &p_padding) {
	padding = p_padding;
}

const Vector2 &VirtualJoystick::get_padding() const {
	return padding;
}

const Vector2 &VirtualJoystick::get_motion() const {
	return motion;
}
