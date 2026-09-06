#include "dynamic_list.hpp"

#include "imtk/drag_drop.hpp"
#include "imtk/id_scope.hpp"

namespace imtk::w
{
	static res::icon_id drag_icon;

	dynamic_list_header::dynamic_list_header(list_model& model)
		: model(model)
	{
		create_button.config.str_id = "##+";
		delete_button.config.str_id = "##-";
		clear_button.config.str_id = "##x";
	}

	dynamic_list_header::dynamic_list_header(list_model& model, const dynamic_list_header& o)
		: model(model), create_button(o.create_button), delete_button(o.delete_button), clear_button(o.clear_button)
	{
	}

	dynamic_list_header::dynamic_list_header(list_model& model, dynamic_list_header&& o) noexcept
		: model(model), create_button(std::move(o.create_button)), delete_button(std::move(o.delete_button)), clear_button(std::move(o.clear_button))
	{
	}

	dynamic_list_header& dynamic_list_header::operator=(const dynamic_list_header& o)
	{
		if (this != &o)
		{
			create_button = o.create_button;
			delete_button = o.delete_button;
			clear_button = o.clear_button;
		}

		return *this;
	}
	
	dynamic_list_header& dynamic_list_header::operator=(dynamic_list_header&& o) noexcept
	{
		if (this != &o)
		{
			create_button = std::move(o.create_button);
			delete_button = std::move(o.delete_button);
			clear_button = std::move(o.clear_button);
		}

		return *this;
	}

	item_result dynamic_list_header::draw_impl()
	{
		item_result result;
		item_result subresult;
		
		subresult = create_button.draw();
		result |= subresult;
		if (subresult)
			model.defer_append();

		if (auto d = disabled(model.size() == 0))
		{
			ImGui::SameLine();
			subresult = delete_button.draw();
			result |= subresult;
			if (subresult)
				model.defer_delete();

			ImGui::SameLine();
			subresult = clear_button.draw();
			result |= subresult;
			if (subresult)
				model.defer_resize(0);
		}

		return result;
	}

	struct dynamic_row_payload : public drag_droppable_pod<dynamic_row_payload>
	{
		const list_model* identity;
		size_t index;

		dynamic_row_payload(const list_model* identity, size_t index)
			: identity(identity), index(index)
		{
		}
	};

	dynamic_row::dynamic_row(list_model& model, icon_button& drag_button, size_t index, const char* str_id)
		: _model(model), _drag_button(drag_button), _index(index)
	{
		_cursor = ImGui::GetCursorScreenPos();
		_size = ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFrameHeight());
		_child = std::make_unique<child>(str_id, _size, ImGuiChildFlags_AutoResizeX);

		if (*_child)
		{
			_visible = true;

			if (_drag_button.draw()) // TODO add result to dynamic_row.result to use in dynamic_list_body
				on_select();

			if (auto _ = drag_drop_source())
			{
				on_select();

				send_drag_drop_payload(dynamic_row_payload(&_model, _index));
				ImGui::TextUnformatted("Move row");
			}

			if (auto target = drag_drop_target())
			{
				if (auto payload = target.accept<dynamic_row_payload>())
				{
					if ((*payload)->identity == &_model && (*payload)->index != _index)
						_model.defer_move((*payload)->index, _index);
				}
			}

			ImGui::SameLine();
		}
	}

	dynamic_row::~dynamic_row()
	{
		_child.reset();
		if (ImGui::IsItemClicked())
			on_select();

		if (_model.index() == _index)
			ImGui::GetWindowDrawList()->AddRectFilled(_cursor, _cursor + _size, ImGui::GetColorU32(ImGuiCol_FrameBgHovered));
		else if (_model.index_selected(_index))
			ImGui::GetWindowDrawList()->AddRectFilled(_cursor, _cursor + _size, ImGui::GetColorU32(ImGuiCol_FrameBgHovered, 0.5f));
	}

	dynamic_row::operator bool() const
	{
		return _visible;
	}

	void dynamic_row::on_select()
	{
		_model.on_select(_index, ImGui::GetIO().KeyCtrl, ImGui::GetIO().KeyShift);
	}

	size_t dynamic_row::index() const
	{
		return _index;
	}

	dynamic_list_body::dynamic_list_body(list_model& model)
		: model(model)
	{
		drag_button.config.str_id = "##Drag";
		drag_button.config.tooltip = "Drag item";
		if (drag_icon)
			drag_button.config.icon = drag_icon;
	}

	dynamic_list_body::dynamic_list_body(list_model& model, const dynamic_list_body& o)
		: model(model), row_draw(o.row_draw), drag_button(o.drag_button)
	{
	}

	dynamic_list_body::dynamic_list_body(list_model& model, dynamic_list_body&& o) noexcept
		: model(model), row_draw(std::move(o.row_draw)), drag_button(std::move(o.drag_button))
	{
	}

	dynamic_list_body& dynamic_list_body::operator=(const dynamic_list_body& o)
	{
		if (this != &o)
		{
			row_draw = o.row_draw;
			drag_button = o.drag_button;
		}

		return *this;
	}

	dynamic_list_body& dynamic_list_body::operator=(dynamic_list_body&& o) noexcept
	{
		if (this != &o)
		{
			row_draw = std::move(o.row_draw);
			drag_button = std::move(o.drag_button);
		}

		return *this;
	}

	item_result dynamic_list_body::draw_impl()
	{
		item_result result;

		for (size_t i = 0; i < model.size(); ++i)
		{
			id_scope scope(i);

			if (auto row = dynamic_row(model, drag_button, i, "Row"))
			{
				auto row_result = row_draw(row);
				if (row_result.state.left_clicked() || row_result.state.focused())
					row.on_select();
				result |= row_result;
			}
		}

		if (ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows) && !ImGui::GetIO().WantTextInput && ImGui::Shortcut(ImGuiKey_Delete))
			model.defer_delete();

		return result;
	}

	void assign_drag_icon(res::icon_id icon)
	{
		drag_icon = icon;
	}
}
