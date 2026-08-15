#include "simple_widget.hpp"

#include "imtk/id_scope.hpp"
#include "imtk/wrapper.hpp"

#include "imtk/prop/common_views.hpp"

// TODO add flags to structs (also in combo_widget)

namespace imtk::w
{
	item_result simple_widget<bool>::draw_impl()
	{
		id_scope scope(&data);
		auto result = prefix_label(config.label);

		result |= item_result::query(ImGui::Checkbox("", &data));
		
		result.modified |= check_property(std::make_unique<prop::simple_view<bool>>(data));
		return result;
	}

	item_result simple_widget<int>::draw_impl()
	{
		id_scope scope(&data);
		auto result = prefix_label(config.label);

		const int og = data;
		result |= item_result::query(ImGui::InputInt("", &data));
		if (imp::clamp(data, config.min, config.max))
			result.modified = data != og;
		
		result.modified |= check_property(std::make_unique<prop::simple_view<int>>(data));
		return result;
	}

	item_result simple_widget<float>::draw_impl()
	{
		id_scope scope(&data);
		auto result = prefix_label(config.label);

		const float og = data;
		result |= item_result::query(ImGui::InputFloat("", &data));
		if (imp::clamp(data, config.min, config.max))
			result.modified = data != og;

		result.modified |= check_property(std::make_unique<prop::simple_view<float>>(data));
		return result;
	}

	item_result simple_widget<double>::draw_impl()
	{
		id_scope scope(&data);
		auto result = prefix_label(config.label);

		const double og = data;
		result |= item_result::query(ImGui::InputDouble("", &data));
		if (imp::clamp(data, config.min, config.max))
			result.modified = data != og;

		result.modified |= check_property(std::make_unique<prop::simple_view<double>>(data));
		return result;
	}

	item_result simple_widget<glm::vec2>::draw_impl()
	{
		id_scope scope(&data);
		auto result = prefix_label(config.label);

		const glm::vec2 og = data;
		result |= item_result::query(ImGui::InputFloat2("", glm::value_ptr(data)));
		if (imp::clamp(glm::value_ptr(data), data.length(), config.min, config.max))
			result.modified = data != og;

		result.modified |= check_property(std::make_unique<prop::simple_view<glm::vec2>>(data));
		return result;
	}

	item_result simple_widget<glm::vec3>::draw_impl()
	{
		id_scope scope(&data);
		auto result = prefix_label(config.label);

		const glm::vec3 og = data;
		result |= item_result::query(ImGui::InputFloat3("", glm::value_ptr(data)));
		if (imp::clamp(glm::value_ptr(data), data.length(), config.min, config.max))
			result.modified = data != og;

		result.modified |= check_property(std::make_unique<prop::simple_view<glm::vec3>>(data));
		return result;
	}

	item_result simple_widget<glm::vec4>::draw_impl()
	{
		id_scope scope(&data);
		auto result = prefix_label(config.label);

		const glm::vec4 og = data;
		result |= item_result::query(ImGui::InputFloat4("", glm::value_ptr(data)));
		if (imp::clamp(glm::value_ptr(data), data.length(), config.min, config.max))
			result.modified = data != og;

		result.modified |= check_property(std::make_unique<prop::simple_view<glm::vec4>>(data));
		return result;
	}

	item_result simple_widget<std::string>::draw_impl()
	{
		id_scope scope(&data);
		auto result = prefix_label(config.label);

		result |= item_result::query(controls::input_text("", data));

		result.modified |= check_property(std::make_unique<prop::simple_view<std::string>>(data));
		return result;
	}
}
