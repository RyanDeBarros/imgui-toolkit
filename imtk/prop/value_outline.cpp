#include "value_outline.hpp"

#include "imtk/prop/property_grid.hpp"

namespace imtk::prop
{
	value_outline::value_outline()
	{
		_begin_listener = value::on_value_begin().subscribe([this]() { on_begin_outline(); });
		_end_listener = value::on_value_end().subscribe([this]() { on_end_outline(); });
	}
	
	value_outline::value_outline(const value_outline& o)
		: _begin(o._begin), _end(o._end)
	{
		_begin_listener = value::on_value_begin().subscribe([this]() { on_begin_outline(); });
		_end_listener = value::on_value_end().subscribe([this]() { on_end_outline(); });
	}

	value_outline::value_outline(value_outline&& o) noexcept
		: _begin(o._begin), _end(o._end)
	{
		_begin_listener = value::on_value_begin().subscribe([this]() { on_begin_outline(); });
		_end_listener = value::on_value_end().subscribe([this]() { on_end_outline(); });
	}

	value_outline& value_outline::operator=(const value_outline& o)
	{
		if (this != &o)
		{
			_begin = o._begin;
			_end = o._end;
		}

		return *this;
	}

	value_outline& value_outline::operator=(value_outline&& o) noexcept
	{
		if (this != &o)
		{
			_begin = o._begin;
			_end = o._end;
		}

		return *this;
	}

	value_outline::operator bool() const
	{
		return true;
	}

	void value_outline::draw(ImU32 color, outline::config cfg) const
	{
		ImGui::GetWindowDrawList()->AddRect(_begin, _end, color, cfg.rounding, cfg.flags, cfg.border);
	}

	void value_outline::on_begin_outline()
	{
		_begin = ImGui::GetCursorScreenPos();
	}

	void value_outline::on_end_outline()
	{
		_end = { ImGui::GetItemRectMax().x, ImGui::GetItemRectMin().y + ImGui::GetFrameHeight() };
	}

	void value_outline::kill()
	{
		_begin_listener.detach();
		_end_listener.detach();
	}
}
