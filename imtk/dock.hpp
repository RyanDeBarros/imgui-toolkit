#pragma once

#include <memory>
#include <string>
#include <vector>

#include <imgui.h>

namespace imtk
{
	class dock_node
	{
		friend class dock;

		std::vector<std::string> _names;
		std::unique_ptr<dock_node> _first = nullptr;
		std::unique_ptr<dock_node> _second = nullptr;
		ImGuiDir _direction = ImGuiDir_None;
		float _split_factor = 0.5f;

		dock_node() = default;

		void split_layout(ImGuiID id) const;

	public:
		void setup_layout(ImGuiID dockspace_id) const;
	};

	class dock
	{
	public:
		static std::unique_ptr<dock_node> make_branch(ImGuiDir direction, std::unique_ptr<dock_node>&& first, std::unique_ptr<dock_node>&& second, float split_factor = 0.5f);
		static std::unique_ptr<dock_node> make_leaf(std::vector<std::string>&& names);
	};
}
