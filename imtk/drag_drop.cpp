#include "drag_drop.hpp"

#include <string>

namespace imtk
{
	std::string drag_drop_type_repr(imp::type_erasure type_erasure)
	{
		return "imtk-" + std::to_string(type_erasure);
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
