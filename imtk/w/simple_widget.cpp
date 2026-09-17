#include "simple_widget.hpp"

#include "imtk/id_scope.hpp"
#include "imtk/controls.hpp"

namespace imtk::w
{
	static std::unordered_map<unsigned char, std::string> precision_formats;

	static const char* precision_format(unsigned char precision)
	{
		auto it = precision_formats.find(precision);
		if (it != precision_formats.end())
			return it->second.c_str();
		else
			return precision_formats.emplace(precision, "%." + std::to_string(precision) + "f").first->second.c_str();
	}

	item_result bound_widget<bool>::draw_impl()
	{
		id_scope scope(&data);
		auto result = prefix_label(config.label.c_str());

		result |= item_result::query(ImGui::Checkbox("", &data));
		
		result.modified |= prop::grid::check_property(std::make_unique<prop::simple_view<bool>>(data));
		return result;
	}

	item_result bound_widget<int>::draw_impl()
	{
		id_scope scope(&data);
		auto result = prefix_label(config.label.c_str());

		const int og = data;
		result |= item_result::query(ImGui::InputInt("", &data, config.step, config.step_fast, config.flags));
		if (imp::clamp(data, config.min, config.max))
			result.modified = data != og;
		
		result.modified |= prop::grid::check_property(std::make_unique<prop::simple_view<int>>(data));
		return result;
	}

	item_result bound_widget<float>::draw_impl()
	{
		id_scope scope(&data);
		auto result = prefix_label(config.label.c_str());

		const float og = data;
		result |= item_result::query(ImGui::InputFloat("", &data, config.step, config.step_fast, precision_format(config.precision), config.flags));
		if (imp::clamp(data, config.min, config.max))
			result.modified = data != og;

		result.modified |= prop::grid::check_property(std::make_unique<prop::simple_view<float>>(data));
		return result;
	}

	item_result bound_widget<double>::draw_impl()
	{
		id_scope scope(&data);
		auto result = prefix_label(config.label.c_str());

		const double og = data;
		result |= item_result::query(ImGui::InputDouble("", &data, config.step, config.step_fast, precision_format(config.precision), config.flags));
		if (imp::clamp(data, config.min, config.max))
			result.modified = data != og;

		result.modified |= prop::grid::check_property(std::make_unique<prop::simple_view<double>>(data));
		return result;
	}

	item_result bound_widget<glm::vec2>::draw_impl()
	{
		id_scope scope(&data);
		auto result = prefix_label(config.label.c_str());

		const glm::vec2 og = data;
		result |= item_result::query(ImGui::InputFloat2("", glm::value_ptr(data), precision_format(config.precision), config.flags));
		if (imp::clamp(glm::value_ptr(data), data.length(), config.min, config.max))
			result.modified = data != og;

		result.modified |= prop::grid::check_property(std::make_unique<prop::simple_view<glm::vec2>>(data));
		return result;
	}

	item_result bound_widget<glm::vec3>::draw_impl()
	{
		id_scope scope(&data);
		auto result = prefix_label(config.label.c_str());

		const glm::vec3 og = data;
		result |= item_result::query(ImGui::InputFloat3("", glm::value_ptr(data), precision_format(config.precision), config.flags));
		if (imp::clamp(glm::value_ptr(data), data.length(), config.min, config.max))
			result.modified = data != og;

		result.modified |= prop::grid::check_property(std::make_unique<prop::simple_view<glm::vec3>>(data));
		return result;
	}

	item_result bound_widget<glm::vec4>::draw_impl()
	{
		id_scope scope(&data);
		auto result = prefix_label(config.label.c_str());

		const glm::vec4 og = data;
		result |= item_result::query(ImGui::InputFloat4("", glm::value_ptr(data), precision_format(config.precision), config.flags));
		if (imp::clamp(glm::value_ptr(data), data.length(), config.min, config.max))
			result.modified = data != og;

		result.modified |= prop::grid::check_property(std::make_unique<prop::simple_view<glm::vec4>>(data));
		return result;
	}

	item_result bound_widget<std::string>::draw_impl()
	{
		id_scope scope(&data);
		auto result = prefix_label(config.label.c_str());

		result |= item_result::query(controls::input_text("", data, config.max_size, config.flags, config.callback, config.user_data));

		result.modified |= prop::grid::check_property(std::make_unique<prop::simple_view<std::string>>(data));
		return result;
	}

	item_result bound_widget<color4>::draw_impl()
	{
		id_scope scope(&data);
		auto result = prefix_label(config.label.c_str());

		result |= item_result::query(ImGui::ColorEdit4("", data.ptr(), config.flags));

		result.modified |= prop::grid::check_property(std::make_unique<prop::simple_view<color4>>(data));
		return result;
	}
}
