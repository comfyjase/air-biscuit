#pragma once

#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/texture_rect.hpp>

namespace godot {

	class Input;

class VirtualJoystick : public TextureRect {
	GDCLASS(VirtualJoystick, TextureRect);

protected:
	static void _bind_methods();

public:
	VirtualJoystick();
	~VirtualJoystick();

	void _ready() override;
	void _input(const Ref<InputEvent> &p_event) override;
	void _process(double p_delta) override;

	void set_padding(const Vector2 &p_padding);
	const Vector2 &get_padding() const;

	const Vector2 &get_motion() const;

private:
	TextureRect *joystick;
	Input *input;
	Vector2 padding;
	Vector2 touch_starting_position;
	Vector2 touch_current_position;
	Vector2 joystick_initial_position;
	Vector2 motion;
	Vector2 centre;
	Vector2 min;
	Vector2 max;
	float touch_area_size;
	float touch_area_size_squared;
	int touch_index;
	bool is_touch_over_joystick_area;
	bool swiping;
	bool resetting;

	void draw_debug();
};

} //namespace godot
