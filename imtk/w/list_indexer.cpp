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

	list_indexer::list_indexer(list_model& model, const list_indexer& o)
		: model(model), create_button(o.create_button), delete_button(o.delete_button), clear_button(o.clear_button)
		, prompt(o.prompt), combo_name(o.combo_name)
	{
	}

	list_indexer::list_indexer(list_model& model, list_indexer&& o) noexcept
		: model(model), create_button(std::move(o.create_button)), delete_button(std::move(o.delete_button)), clear_button(std::move(o.clear_button))
		, prompt(std::move(o.prompt)), combo_name(std::move(o.combo_name))
	{
	}

	list_indexer& list_indexer::operator=(const list_indexer& o)
	{
		if (this != &o)
		{
			create_button = o.create_button;
			delete_button = o.delete_button;
			clear_button = o.clear_button;
			prompt = o.prompt;
			combo_name = o.combo_name;
		}

		return *this;
	}

	list_indexer& list_indexer::operator=(list_indexer&& o) noexcept
	{
		if (this != &o)
		{
			create_button = std::move(o.create_button);
			delete_button = std::move(o.delete_button);
			clear_button = std::move(o.clear_button);
			prompt = std::move(o.prompt);
			combo_name = std::move(o.combo_name);
		}

		return *this;
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

	std::function<std::string(size_t)> make_combo_name_from_prefix(std::string slot_prefix)
	{
		return [slot_prefix = std::move(slot_prefix)](size_t i) { return slot_prefix + " " + std::to_string(i); };
	}

	owned_list_indexer::owned_list_indexer()
		: widget(model)
	{
	}

	owned_list_indexer::owned_list_indexer(const owned_list_indexer& o)
		: model(o.model), widget(model, o.widget)
	{
	}

	owned_list_indexer::owned_list_indexer(owned_list_indexer&& o) noexcept
		: model(std::move(o.model)), widget(model, std::move(o.widget))
	{
	}

	item_result owned_list_indexer::draw_impl()
	{
		return widget.draw();
	}
}
