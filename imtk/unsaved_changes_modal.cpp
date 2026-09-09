#include "unsaved_changes_modal.hpp"

namespace imtk
{
	unsaved_changes_modal::unsaved_changes_modal(std::string str_id, std::vector<std::string> description)
		: pop("Unsaved Changes##" + str_id, imtk::popup_config{.center_window = imtk::center_window::always, .window_flags = ImGuiWindowFlags_AlwaysAutoResize})
		, description(std::move(description))
	{
	}

	unsaved_changes_modal::result unsaved_changes_modal::draw()
	{
		result res = result::no_draw;

		if (auto d = pop.draw())
		{
			res = result::no_interaction;

			for (const auto& line : description)
				ImGui::TextUnformatted(line.c_str());

			if (ImGui::Button("Save Changes"))
			{
				d.close();
				res = result::save_changes;
			}

			ImGui::SameLine();
			if (ImGui::Button("Discard Changes"))
			{
				d.close();
				res = result::discard_changes;
			}

			ImGui::SameLine();
			if (ImGui::Button("Cancel Close"))
			{
				d.close();
				res = result::cancel_close;
			}
		}

		return res;
	}

	bool unsaved_changes_modal::closing(result res)
	{
		return res == result::save_changes || res == result::discard_changes;
	}
}
