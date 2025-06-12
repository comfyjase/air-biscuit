#include "player.h"

#include <godot_cpp/classes/animation_player.hpp>
#include <godot_cpp/classes/animation_tree.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/vector3.hpp>

#if IMGUI_ENABLED
#include <imgui-godot.h>
#endif

#include "player_camera.h"
#include "player_input.h"

using namespace godot;

void Player::_bind_methods() {
#if IMGUI_ENABLED
	ClassDB::bind_method(D_METHOD("draw_debug"), &Player::draw_debug);
#endif
}

Player::Player() :
		CharacterBody(),
		camera(nullptr),
		input(nullptr) {
}

Player::~Player() {
}

void Player::_ready() {
	if (Engine::get_singleton()->is_editor_hint()) {
		set_process_input(false);
		set_process(false);
		set_physics_process(false);
		return;
	}

	camera = get_node<PlayerCamera>("CameraBase/CameraRot/SpringArm3D/Camera");
	ERR_FAIL_NULL_MSG(camera, "Failed to get player camera child node");

	input = get_node<PlayerInput>("Input");
	ERR_FAIL_NULL_MSG(input, "Failed to get player input child node");

	animation_player = get_node<AnimationPlayer>("Shibby/AnimationPlayer");
	ERR_FAIL_NULL_MSG(animation_player, "Failed to get animation player");

	model = get_node<Node3D>("Shibby");
	ERR_FAIL_NULL_MSG(animation_player, "Failed to get player model");

	initialize();

	//animation_tree = get_node<AnimationTree>("AnimationTree");
	//ERR_FAIL_NULL_MSG(animation_tree, "Failed to get animation tree");
}

void Player::_physics_process(double p_delta) {
	process_input(p_delta);
}

void Player::animate(int p_animation, double p_delta) {
	if (p_animation == ANIMATIONS::IDLE) {
		animation_player->play("Idle");
	} else if (p_animation == ANIMATIONS::WALK) {
		animation_player->play("Walk");
	}
}

void Player::process_input(double p_delta) {
#if 0
	if (input->get_motion().is_zero_approx()) {
		animate(ANIMATIONS::IDLE, p_delta);
		return;
	}

	motion = motion.lerp(input->get_motion(), speed * p_delta);

	const Basis &camera_basis = camera->get_camera_basis();
	Vector3 camera_x = camera_basis.get_column(0);
	Vector3 camera_z = camera_basis.get_column(2); 

	camera_x.y = 0.0f;
	camera_x = camera_x.normalized();
	camera_z.y = 0.0f;
	camera_z = camera_z.normalized();

	Vector3 target = (camera_x * motion.x) + (camera_z * motion.y);
	if (target.length() > 0.001f) {
		Quaternion from = orientation.get_basis().get_rotation_quaternion();
		Quaternion to = Transform3D().looking_at(target, Vector3(0.0f, 1.0f, 0.0f)).get_basis().get_rotation_quaternion();
		orientation.basis = Basis(from.slerp(to, p_delta * rotation_speed));
	}

	animate(ANIMATIONS::WALK, p_delta);

	root_motion = Transform3D(animation_player->get_root_motion_rotation(), animation_player->get_root_motion_position());
	orientation *= root_motion;

	Vector3 h_velocity = orientation.origin / p_delta;
	Vector3 new_velocity = h_velocity;
	new_velocity.y = 0.0f;

	set_velocity(new_velocity);
	set_up_direction(Vector3(0.0f, 1.0f, 0.0f));
	move_and_slide();

	orientation.origin = Vector3();
	orientation = orientation.orthonormalized();

	//model->set_global_basis(orientation.basis);
	set_global_basis(orientation.basis);

#else
	if(input->get_motion().is_zero_approx()) {
		animate(ANIMATIONS::IDLE, p_delta);
		return;
	}

	const Basis &camera_basis = camera->get_camera_basis();
	Vector3 camera_x = camera_basis.get_column(0);
	Vector3 camera_z = camera_basis.get_column(2);

	camera_x.y = 0.0f;
	camera_x = camera_x.normalized();
	camera_z.y = 0.0f;
	camera_z = camera_z.normalized();

	Vector3 target = (camera_x * motion.x) + (camera_z * motion.y);
	if (target.length() > 0.001f) {
		Quaternion from = orientation.get_basis().get_rotation_quaternion();
		Quaternion to = Transform3D().looking_at(target, Vector3(0.0f, 1.0f, 0.0f)).get_basis().get_rotation_quaternion();
		orientation.basis = Basis(from.slerp(to, p_delta * rotation_speed));
	}

	Vector3 new_velocity = get_velocity();

	//if (!is_on_floor()) {
	//	new_velocity += get_gravity() * p_delta;
	//}

	//if (input->is_jumping() && is_on_floor()) {
	//	new_velocity.y += jump_speed;
	//}

	const float horizontal_rotation = camera->get_basis().get_euler().y;

	const Vector3 &direction = get_transform().get_basis().xform(Vector3(input->get_motion().x, 0.0f, input->get_motion().y)).rotated(Vector3(0.0f, 1.0f, 0.0f), horizontal_rotation).normalized();
	if (!direction.is_zero_approx()) {
		new_velocity.x = direction.x * speed;
		new_velocity.z = direction.z * speed;

		//Node3D *look_at_node = get_node<Node3D>("LookAtPoint");
		//look_at(look_at_node->get_transform().get_origin());
	} else {
		new_velocity.x = Math::move_toward(get_velocity().x, 0.0f, speed);
		new_velocity.z = Math::move_toward(get_velocity().z, 0.0f, speed);
	}

	animate(ANIMATIONS::WALK, p_delta);

	set_velocity(new_velocity);

	/*if (!get_velocity().is_zero_approx()) {
		Vector3 new_estimate_position = get_global_position() + get_velocity();
		Vector3 look_at_position = get_global_transform().get_basis().get_column(2).normalized() * 10.0f;
		look_at(look_at_position);
	}*/

	move_and_slide();

	Quaternion new_rotation = orientation.get_basis().get_rotation_quaternion();
	model->set_quaternion(new_rotation);

	if (!new_velocity.is_zero_approx()) {
		//Vector3 forward = (get_global_position() - camera->get_global_position()).normalized();
		//forward.y = 0.0f;

		/*Vector3 look_at_position = get_global_position() + (direction * 3.0f);
		look_at_position.y = get_global_position().y;
		look_at(look_at_position);
		orthonormalize();*/

		//const Quaternion& target_quaternion = Quaternion(get_basis()).normalized();
		//rotate_y(-target_quaternion.x);
		//orthonormalize();

		//constexpr float distance_in_front = 50.0f;
		//Vector3 position_in_front_of_player = get_global_transform().get_origin() + (direction * distance_in_front);
		//position_in_front_of_player.y = get_global_transform().get_origin().y;
		//look_at(position_in_front_of_player);
	}
#endif
}

void Player::draw_debug() {
#if IMGUI_ENABLED
	ImGui::Text("New stuff is being added");

	ImGui::Text("Input Motion");

	float motion_x = input->get_motion().x;
	float motion_y = input->get_motion().y;

	ImGui::DragFloat("X: ##motion", &motion_x);
	ImGui::DragFloat("Y: ##motion", &motion_y);

	ImGui::Separator();

	ImGui::Text("Velocity");

	float velocity_x = get_velocity().x;
	float velocity_y = get_velocity().y;
	float velocity_z = get_velocity().z;

	ImGui::DragFloat("X: ##velocity", &velocity_x);
	ImGui::DragFloat("Y: ##velocity", &velocity_y);
	ImGui::DragFloat("Z: ##velocity", &velocity_z);
#endif
}
