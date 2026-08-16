#include "simple_widget.hpp"

#include "imtk/id_scope.hpp"
#include "imtk/wrapper.hpp"

#include "imtk/prop/common_views.hpp"
#include "imtk/prop/property_grid.hpp"

namespace imtk::w
{
	item_result bound_widget<bool>::draw_impl()
	{
		id_scope scope(&data);
		auto result = prefix_label(config.label);

		result |= item_result::query(ImGui::Checkbox("", &data));
		
		result.modified |= prop::check_property(std::make_unique<prop::simple_view<bool>>(data));
		return result;
	}

	item_result bound_widget<int>::draw_impl()
	{
		id_scope scope(&data);
		auto result = prefix_label(config.label);

		const int og = data;
		result |= item_result::query(ImGui::InputInt("", &data, config.step, config.step_fast, config.flags));
		if (imp::clamp(data, config.min, config.max))
			result.modified = data != og;
		
		result.modified |= prop::check_property(std::make_unique<prop::simple_view<int>>(data));
		return result;
	}

	item_result bound_widget<float>::draw_impl()
	{
		id_scope scope(&data);
		auto result = prefix_label(config.label);

		const float og = data;
		result |= item_result::query(ImGui::InputFloat("", &data, config.step, config.step_fast, config.format, config.flags));
		if (imp::clamp(data, config.min, config.max))
			result.modified = data != og;

		result.modified |= prop::check_property(std::make_unique<prop::simple_view<float>>(data));
		return result;
	}

	item_result bound_widget<double>::draw_impl()
	{
		id_scope scope(&data);
		auto result = prefix_label(config.label);

		const double og = data;
		result |= item_result::query(ImGui::InputDouble("", &data, config.step, config.step_fast, config.format, config.flags));
		if (imp::clamp(data, config.min, config.max))
			result.modified = data != og;

		result.modified |= prop::check_property(std::make_unique<prop::simple_view<double>>(data));
		return result;
	}

	item_result bound_widget<glm::vec2>::draw_impl()
	{
		id_scope scope(&data);
		auto result = prefix_label(config.label);

		const glm::vec2 og = data;
		result |= item_result::query(ImGui::InputFloat2("", glm::value_ptr(data), config.format, config.flags));
		if (imp::clamp(glm::value_ptr(data), data.length(), config.min, config.max))
			result.modified = data != og;

		result.modified |= prop::check_property(std::make_unique<prop::simple_view<glm::vec2>>(data));
		return result;
	}

	item_result bound_widget<glm::vec3>::draw_impl()
	{
		id_scope scope(&data);
		auto result = prefix_label(config.label);

		const glm::vec3 og = data;
		result |= item_result::query(ImGui::InputFloat3("", glm::value_ptr(data), config.format, config.flags));
		if (imp::clamp(glm::value_ptr(data), data.length(), config.min, config.max))
			result.modified = data != og;

		result.modified |= prop::check_property(std::make_unique<prop::simple_view<glm::vec3>>(data));
		return result;
	}

	item_result bound_widget<glm::vec4>::draw_impl()
	{
		id_scope scope(&data);
		auto result = prefix_label(config.label);

		const glm::vec4 og = data;
		result |= item_result::query(ImGui::InputFloat4("", glm::value_ptr(data), config.format, config.flags));
		if (imp::clamp(glm::value_ptr(data), data.length(), config.min, config.max))
			result.modified = data != og;

		result.modified |= prop::check_property(std::make_unique<prop::simple_view<glm::vec4>>(data));
		return result;
	}

	item_result bound_widget<std::string>::draw_impl()
	{
		id_scope scope(&data);
		auto result = prefix_label(config.label);

		result |= item_result::query(controls::input_text("", data, config.max_size, config.flags, config.callback, config.user_data));

		result.modified |= prop::check_property(std::make_unique<prop::simple_view<std::string>>(data));
		return result;
	}

	item_result bound_widget<color4>::draw_impl()
	{
		id_scope scope(&data);
		auto result = prefix_label(config.label);

		result |= item_result::query(ImGui::ColorEdit4("", data.ptr(), config.flags));

		result.modified |= prop::check_property(std::make_unique<prop::simple_view<color4>>(data));
		return result;
	}
}
