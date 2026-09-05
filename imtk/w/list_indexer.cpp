#include "list_indexer.hpp"

#include "imtk/simple_scopes.hpp"
#include "imtk/controls.hpp"

namespace imtk::w
{
	list_indexer::list_indexer(list_model& model)
		: model(model)
	{
		create_button.config.str_id = "##+";
		delete_button.config.str_id = "##-";
		clear_button.config.str_id = "##x";
	}

	item_result list_indexer::draw_impl()
	{
		item_result result;

		style_color sc(ImGuiCol_ChildBg, ImGui::GetColorU32(ImGuiCol_FrameBg, 0.75f));

		if (auto _ = child(prompt, ImVec2(0, 0), ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_Borders))
		{
			ImGui::TextUnformatted(prompt.c_str());
			ImGui::SameLine();

			item_result subresult;

			std::vector<std::string> slot_names;
			slot_names.reserve(model.size());
			for (int i = 0; i < model.size(); ++i)
				slot_names.push_back(combo_name(i));

			int slot = model.index();
			result |= controls::combo("##SelectSlot", slot, slot_names);
			model.on_select(slot, false, false);

			ImGui::SameLine();
			subresult = create_button.draw();
			result |= subresult;
			if (subresult.modified)
				model.defer_append();

			ImGui::SameLine();
			subresult = delete_button.draw();
			result |= subresult;
			if (subresult.modified)
				model.defer_delete();

			ImGui::SameLine();
			subresult = clear_button.draw();
			result |= subresult;
			if (subresult.modified)
				model.defer_clear();
		}

		return result;
	}

	void list_indexer::configure_buttons(icon_image create_icon, std::string create_tooltip,
		icon_image delete_icon, std::string delete_tooltip, icon_image clear_icon, std::string clear_tooltip)
	{
		create_button.config.icon = create_icon;
		create_button.config.tooltip = std::move(create_tooltip);
		delete_button.config.icon = delete_icon;
		delete_button.config.tooltip = std::move(delete_tooltip);
		clear_button.config.icon = clear_icon;
		clear_button.config.tooltip = std::move(clear_tooltip);
	}

	std::function<std::string(size_t)> make_combo_name_from_prefix(std::string slot_prefix)
	{
		return [slot_prefix = std::move(slot_prefix)](size_t i) { return slot_prefix + " " + std::to_string(i); };
	}

	owned_list_indexer::owned_list_indexer()
		: widget(model)
	{
	}

	item_result owned_list_indexer::draw_impl()
	{
		return widget.draw();
	}
}
