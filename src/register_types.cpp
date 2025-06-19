#include "register_types.h"

#include <gdextension_interface.h>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#if IMGUI_ENABLED
#include <imgui-godot.h>
#endif

#if TESTS_ENABLED
#include "tests/game_test_node.h"
#endif

#include "build_information.h"
#include "camera.h"
#include "character_body.h"

// Player
#include "player.h"
#include "player_camera.h"
#include "player_input.h"

// States
#include "state_attack.h"
#include "state_locomotion.h"
#include "state_machine.h"

#include "function_library.h"

using namespace godot;

void initialize_gdextension_types(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

#if IMGUI_ENABLED
	ImGui::Godot::SyncImGuiPtrs();
#endif

	// Singletons.
	GDREGISTER_RUNTIME_CLASS(FunctionLibrary);
	FunctionLibrary::create_singleton();
	Engine::get_singleton()->register_singleton(StringName("FunctionLibrary"), FunctionLibrary::get_singleton());

	// Unit tests.
#if TESTS_ENABLED
	GDREGISTER_RUNTIME_CLASS(GameTest);
#endif

	// Debug.
	GDREGISTER_RUNTIME_CLASS(BuildInformation);

	// Shared classes.
	GDREGISTER_CLASS(Camera);
	GDREGISTER_CLASS(CharacterBody);

	GDREGISTER_CLASS(PlayerCamera);
	GDREGISTER_CLASS(PlayerInput);
	GDREGISTER_CLASS(Player);

	GDREGISTER_CLASS(State);
	GDREGISTER_CLASS(StateAnimated);
	GDREGISTER_CLASS(StateMachine);
	GDREGISTER_CLASS(StateLocomotion);
	GDREGISTER_CLASS(StateAttack);

	// Project specific classes.
	// Will go here...
}

void uninitialize_gdextension_types(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	// Singletons.
	Engine::get_singleton()->unregister_singleton("FunctionLibrary");
	FunctionLibrary::free_singleton();
}

extern "C" {
// Initialization
GDExtensionBool GDE_EXPORT game_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
	GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);
	init_obj.register_initializer(initialize_gdextension_types);
	init_obj.register_terminator(uninitialize_gdextension_types);
	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

	return init_obj.init();
}
}
