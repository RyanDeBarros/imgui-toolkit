#include "list_indexer.hpp"

#include "imtk/simple_scopes.hpp"
#include "imtk/controls.hpp"

namespace imtk::w
{
	static res::icon_id create_icon;
	static res::icon_id delete_icon;
	static res::icon_id clear_icon;

	static void configure_buttons(list_indexer& li)
	{
		li.create_button.config.str_id = "##+";
		li.create_button.config.icon = create_icon;
		li.delete_button.config.str_id = "##-";
		li.delete_button.config.icon = delete_icon;
		li.clear_button.config.str_id = "##x";
		li.clear_button.config.icon = clear_icon;
	}

	list_indexer::list_indexer(list_model& model)
		: model(model)
	{
		configure_buttons(*this);
	}

	list_indexer::list_indexer(list_model& model, config cfg, std::function<std::string(size_t)> combo_name)
		: model(model), combo_name(std::move(combo_name))
	{
		configure_buttons(*this);

		create_button.config.tooltip = std::move(cfg.create_tooltip);
		delete_button.config.tooltip = std::move(cfg.delete_tooltip);
		clear_button.config.tooltip = std::move(cfg.clear_tooltip);

		prompt = std::move(cfg.prompt);
	}

	list_indexer::list_indexer(list_model& model, config cfg, std::string combo_slot_prefix)
		: model(model), combo_name(make_combo_name_from_prefix(combo_slot_prefix))
	{
		configure_buttons(*this);

		create_button.config.tooltip = std::move(cfg.create_tooltip);
		delete_button.config.tooltip = std::move(cfg.delete_tooltip);
		clear_button.config.tooltip = std::move(cfg.clear_tooltip);

		prompt = std::move(cfg.prompt);
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

	owned_list_indexer::owned_list_indexer(list_indexer::config cfg, std::function<std::string(size_t)> combo_name)
		: widget(model, std::move(cfg), std::move(combo_name))
	{
	}

	owned_list_indexer::owned_list_indexer(list_indexer::config cfg, std::string combo_slot_prefix)
		: widget(model, std::move(cfg), std::move(combo_slot_prefix))
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

	void assign_list_indexer_icons(res::icon_id create_icon_, res::icon_id delete_icon_, res::icon_id clear_icon_)
	{
		create_icon = create_icon_;
		delete_icon = delete_icon_;
		clear_icon = clear_icon_;
	}
}
