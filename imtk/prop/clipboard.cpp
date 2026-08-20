#include "clipboard.hpp"

#include "imtk/simple_scopes.hpp"

#include <string>
#include <variant>

// TODO v9.3 serialize to OS clipboard?
namespace imtk::prop::clipboard
{
	struct payload_node
	{
		std::variant<std::monostate, payload, std::vector<std::unique_ptr<payload_node>>> sub;
	};

	static payload_node instance;
	static size_t session_context_menu = 0;

	void clear()
	{
		instance = {};
	}

	void store(const iview& view)
	{
		instance = {};
		instance.sub = view.dump();
	}

	bool can_paste(const iview& view)
	{
		if (auto pld = std::get_if<payload>(&instance.sub))
			return view.can_load(*pld);
		else
			return false;
	}

	bool try_paste(const iview& view)
	{
		if (auto pld = std::get_if<payload>(&instance.sub))
			return view.try_load(*pld);
		else
			return false;
	}

	bool context_menu_items(const iview& view)
	{
		if (ImGui::MenuItem("Copy"))
			store(view);

		if (auto d = disabled(!can_paste(view)))
		{
			if (ImGui::MenuItem("Paste"))
				return try_paste(view);
		}

		return false;
	}

	// TODO call on every frame rather than in grid constructor?
	void new_session()
	{
		session_context_menu = 0;
	}

	static imtk::context_menu draw_context_menu()
	{
		return context_menu::item("##" + std::to_string(session_context_menu++));
	}

	bool context_menu(const iview& view)
	{
		if (auto _ = draw_context_menu())
			return context_menu_items(view);
		else
			return false;
	}
	
	bool context_menu(const view_generator& generator)
	{
		if (auto _ = draw_context_menu())
			return context_menu_items(*(generator()));
		else
			return false;
	}
}
