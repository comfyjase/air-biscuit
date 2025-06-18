#include "function_library.h"

using namespace godot;

FunctionLibrary *FunctionLibrary::singleton = nullptr;

void FunctionLibrary::_bind_methods() {}

void FunctionLibrary::create_singleton() {
	ERR_FAIL_COND(singleton != nullptr);
	singleton = memnew(FunctionLibrary);
}

void FunctionLibrary::free_singleton() {
	ERR_FAIL_NULL(singleton);
	memdelete(singleton);
	singleton = nullptr;
}

FunctionLibrary* FunctionLibrary::get_singleton() {
	return singleton;
}

FunctionLibrary::FunctionLibrary() : Object() {}

FunctionLibrary::~FunctionLibrary() {}
