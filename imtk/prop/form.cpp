#include "form.hpp"

#include "imtk/simple_scopes.hpp"

#include <memory>

namespace imtk::prop
{
	form* active_form = nullptr;
	std::unique_ptr<child> form_child;
	std::unique_ptr<table> form_table;

	form::form()
	{
		begin_table();
	}

	form::form(form&& other) noexcept
		: _draw_content(other._draw_content), _id_counter(other._id_counter), _scope(std::move(other._scope))
	{
		other._draw_content = false;
		other._id_counter = 0;
		other._valid = false;

		if (&other == active_form)
			active_form = this;
	}

	form::~form()
	{
		if (_valid)
			end_table();
	}

	form* form::active_instance()
	{
		return active_form;
	}

	form::operator bool() const
	{
		return _valid && _draw_content;
	}

	void form::begin_table()
	{
		active_form = this;
		_scope.push(&active_form).push(_id_counter++);

		_draw_content = false;
		form_table.reset();
		form_child = std::make_unique<child>("##imtk-formID", ImVec2(0.f, 0.f), ImGuiChildFlags_AutoResizeY);
		if (*form_child)
		{
			form_table = std::make_unique<table>("", 3, ImGuiTableFlags_BordersInner | ImGuiTableFlags_SizingFixedFit);
			if (*form_table)
			{
				ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed);
				ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch);
				ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, ImGui::GetFrameHeight());
				_draw_content = true;
			}
		}
	}

	void form::end_table()
	{
		form_table.reset();
		form_child.reset();
		_draw_content = false;

		_scope.pop_all();
		active_form = nullptr;
	}

	bool in_form()
	{
		return active_form && *active_form;
	}

	form_pause::form_pause()
		: _form(active_form)
	{
		if (_form)
			_was_drawing_content = _form->_draw_content;

		if (_form)
			_form->end_table();
	}

	form_pause::~form_pause()
	{
		if (_form)
			_form->begin_table();
	}

	form_pause::operator bool() const
	{
		return _form && _form->_valid && _was_drawing_content;
	}
}
