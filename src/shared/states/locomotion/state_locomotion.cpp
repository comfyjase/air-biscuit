#include "state_locomotion.h"

#include <godot_cpp/classes/animation_player.hpp>
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/node3d.hpp>

#include "function_library.h"
#include "state_machine.h"

#if IMGUI_ENABLED
#include <imgui-godot.h>
#endif

using namespace godot;

void StateLocomotion::_bind_methods() {}

StateLocomotion::StateLocomotion() :
		StateAnimated(),
		character_body(nullptr),
		node_to_update_look_at(nullptr),
		input_velocity(),
		rotation(0.0f) {}

StateLocomotion::~StateLocomotion() {}

void StateLocomotion::on_enter() {
	character_body = FunctionLibrary::get_singleton()->find_node<CharacterBody3D>(owner->get_parent());
}

void StateLocomotion::physics_process(double p_delta) {
	if (input_velocity.is_zero_approx()) {
		animate("Idle", p_delta);
		return;
	}

	ERR_FAIL_NULL_MSG(character_body, "StateLocomotion - character_body is nullptr, does the owner of this StateMachine component have a CharacterBody3D node?");
	ERR_FAIL_NULL_MSG(node_to_update_look_at, "StateLocomotion - node_to_update_look_at is nullptr, please call set_node_to_update_look_at(node);");

	Vector3 new_velocity = character_body->get_velocity();

	//if (!character_body->is_on_floor()) {
	//	new_velocity += character_body->get_gravity() * p_delta;
	//}

	const Vector3 &direction = character_body->get_transform().get_basis().xform(Vector3(input_velocity.x, 0.0f, input_velocity.y)).rotated(Vector3(0.0f, 1.0f, 0.0f), rotation).normalized();
	if (!direction.is_zero_approx()) {
		new_velocity.x = direction.x * speed;
		new_velocity.z = direction.z * speed;
	} else {
		new_velocity.x = Math::move_toward(character_body->get_velocity().x, 0.0f, speed);
		new_velocity.z = Math::move_toward(character_body->get_velocity().z, 0.0f, speed);
	}

	animate("Walk", p_delta);

	character_body->set_velocity(new_velocity);
	character_body->move_and_slide();

	Vector3 look_at_position = character_body->get_global_position() + direction;
	look_at_position.y = character_body->get_global_position().y;
	node_to_update_look_at->look_at(look_at_position);
}

void StateLocomotion::on_exit() {
	character_body = nullptr;
}

const String StateLocomotion::get_name() const {
	return "Locomotion";
}

void StateLocomotion::draw_debug() {
#if IMGUI_ENABLED
	ImGui::Text("Velocity");

	float velocity_x = character_body->get_velocity().x;
	float velocity_y = character_body->get_velocity().y;
	float velocity_z = character_body->get_velocity().z;

	ImGui::DragFloat("X: ##velocity_locomotion", &velocity_x);
	ImGui::DragFloat("Y: ##velocity_locomotion", &velocity_y);
	ImGui::DragFloat("Z: ##velocity_locomotion", &velocity_z);
#endif
}

void StateLocomotion::set_node_to_update_look_at(Node3D *p_node) {
	node_to_update_look_at = p_node;
}

void StateLocomotion::set_input_velocity(const Vector2 &p_input_velocity) {
	input_velocity = p_input_velocity;
}

void StateLocomotion::set_rotation(const float p_rotation) {
	rotation = p_rotation;
}

void StateLocomotion::set_speed(const float p_speed) {
	speed = p_speed;
}
