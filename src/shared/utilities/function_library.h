#pragma once

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/variant/typed_array.hpp>
#include <godot_cpp/variant/variant.hpp>

namespace godot {

class FunctionLibrary : public Object {
	GDCLASS(FunctionLibrary, Object);

protected:
	static void _bind_methods();

public:
	static void create_singleton();
	static void free_singleton();
	static FunctionLibrary *get_singleton();

	FunctionLibrary();
	~FunctionLibrary();

	template <typename T>
	T *find_node(Node *root_node) {
		ERR_FAIL_NULL_V_MSG(root_node, nullptr, "FunctionLibrary::find_node<T> parameter root_node is nullptr.");

		T *node = Object::cast_to<T>(root_node);
		if (node) {
			return node;
		}

		TypedArray<Node> children_nodes = root_node->get_children();
		for (int i = 0; i < children_nodes.size(); ++i) {
			Variant child_node_variant = children_nodes[i];

			T *child_node = Object::cast_to<T>(child_node_variant);
			if (child_node) {
				return child_node;
			// Recursively checks children nodes.
			} else {
				child_node = find_node<T>(Object::cast_to<Node>(child_node_variant));
				if (child_node)
					return child_node;
			}
		}

		return nullptr;
	};

private:
	static FunctionLibrary *singleton;
};

} //namespace godot
