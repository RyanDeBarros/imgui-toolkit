#include "drag_drop.hpp"

#include <unordered_map>
#include <string>

namespace imtk
{
	static std::unordered_map<std::type_index, std::string> data_type_reprs;
	static std::unordered_map<std::string, std::type_index> data_type_repr_lut;

	drag_drop_type::drag_drop_type(std::type_index ti)
		: _ti(ti)
	{
	}

	std::string drag_drop_type::repr() const
	{
		auto it = data_type_reprs.find(_ti);
		if (it != data_type_reprs.end())
			return it->second;
		else
		{
			std::string repr = "imtk-" + std::to_string(data_type_reprs.size());
			data_type_reprs.emplace(_ti, repr);
			data_type_repr_lut.emplace(repr, _ti);
			return repr;
		}
	}

	drag_drop_type::operator bool() const
	{
		return _ti != typeid(void);
	}

	drag_drop_source::drag_drop_source(ImGuiDragDropFlags flags)
	{
		_alive = ImGui::BeginDragDropSource(flags);
	}

	drag_drop_source::drag_drop_source(drag_drop_source&& o) noexcept
		: _alive(o._alive)
	{
		o._alive = false;
	}

	drag_drop_source::~drag_drop_source()
	{
		if (_alive)
			ImGui::EndDragDropSource();
	}

	drag_drop_source::operator bool() const
	{
		return _alive;
	}

	drag_drop_target::drag_drop_target()
	{
		_alive = ImGui::BeginDragDropTarget();
	}

	drag_drop_target::drag_drop_target(drag_drop_target&& o) noexcept
		: _alive(o._alive)
	{
		o._alive = false;
	}

	drag_drop_target::~drag_drop_target()
	{
		if (_alive)
			ImGui::EndDragDropTarget();
	}

	drag_drop_target::operator bool() const
	{
		return _alive;
	}
}
