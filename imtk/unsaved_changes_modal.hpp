#pragma once

#include "imtk/popup.hpp"

#include <vector>

namespace imtk
{
	struct unsaved_changes_modal
	{
		popup pop;
		std::vector<std::string> description;

		enum class result
		{
			no_draw,
			no_interaction,
			save_changes,
			discard_changes,
			cancel_close
		};

		unsaved_changes_modal(std::string str_id, std::vector<std::string> description = {});

		result draw();
		static bool closing(result res);
	};
}
