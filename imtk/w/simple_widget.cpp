#include "simple_widget.hpp"

#include "imtk/id_scope.hpp"
#include "imtk/controls.hpp"

namespace imtk::w
{
	item_result bound_widget<bool>::draw_impl()
	{
		id_scope scope(&data);
		auto result = prefix_label(config.label);

		result |= item_result::query(ImGui::Checkbox("", &data));
		
		result.modified |= prop::grid::check_property(std::make_unique<prop::simple_view<bool>>(data));
		return result;
	}

	item_result bound_widget<edit_session<bool>>::draw_impl()
	{
		id_scope scope(&data);
		auto result = prefix_label(config.label);

		data.pre_edit();
		auto edit_result = item_result::query(ImGui::Checkbox("", &data.buffer()));
		result |= edit_result;
		data.post_edit(edit_result.state);

		result.modified |= prop::grid::check_property(std::make_unique<prop::simple_view<edit_session<bool>>>(data));
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
		
		result.modified |= prop::grid::check_property(std::make_unique<prop::simple_view<int>>(data));
		return result;
	}

	item_result bound_widget<edit_session<int>>::draw_impl()
	{
		id_scope scope(&data);
		auto result = prefix_label(config.label);

		data.pre_edit();
		const int og = data.buffer();
		auto edit_result = item_result::query(ImGui::InputInt("", &data.buffer(), config.step, config.step_fast, config.flags));
		result |= edit_result;
		if (imp::clamp(data.buffer(), config.min, config.max))
			result.modified = data.buffer() != og;
		data.post_edit(edit_result.state);

		result.modified |= prop::grid::check_property(std::make_unique<prop::simple_view<edit_session<int>>>(data));
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

		result.modified |= prop::grid::check_property(std::make_unique<prop::simple_view<float>>(data));
		return result;
	}

	item_result bound_widget<edit_session<float>>::draw_impl()
	{
		id_scope scope(&data);
		auto result = prefix_label(config.label);

		data.pre_edit();
		const float og = data.buffer();
		auto edit_result = item_result::query(ImGui::InputFloat("", &data.buffer(), config.step, config.step_fast, config.format, config.flags));
		result |= edit_result;
		if (imp::clamp(data.buffer(), config.min, config.max))
			result.modified = data.buffer() != og;
		data.post_edit(edit_result.state);

		result.modified |= prop::grid::check_property(std::make_unique<prop::simple_view<edit_session<float>>>(data));
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

		result.modified |= prop::grid::check_property(std::make_unique<prop::simple_view<double>>(data));
		return result;
	}

	item_result bound_widget<edit_session<double>>::draw_impl()
	{
		id_scope scope(&data);
		auto result = prefix_label(config.label);

		data.pre_edit();
		const double og = data.buffer();
		auto edit_result = item_result::query(ImGui::InputDouble("", &data.buffer(), config.step, config.step_fast, config.format, config.flags));
		result |= edit_result;
		if (imp::clamp(data.buffer(), config.min, config.max))
			result.modified = data.buffer() != og;
		data.post_edit(edit_result.state);

		result.modified |= prop::grid::check_property(std::make_unique<prop::simple_view<edit_session<double>>>(data));
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

		result.modified |= prop::grid::check_property(std::make_unique<prop::simple_view<glm::vec2>>(data));
		return result;
	}

	item_result bound_widget<edit_session<glm::vec2>>::draw_impl()
	{
		id_scope scope(&data);
		auto result = prefix_label(config.label);

		data.pre_edit();
		const glm::vec2 og = data.buffer();
		auto edit_result = item_result::query(ImGui::InputFloat2("", glm::value_ptr(data.buffer()), config.format, config.flags));
		result |= edit_result;
		if (imp::clamp(glm::value_ptr(data.buffer()), data.buffer().length(), config.min, config.max))
			result.modified = data.buffer() != og;
		data.post_edit(edit_result.state);

		result.modified |= prop::grid::check_property(std::make_unique<prop::simple_view<edit_session<glm::vec2>>>(data));
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

		result.modified |= prop::grid::check_property(std::make_unique<prop::simple_view<glm::vec3>>(data));
		return result;
	}

	item_result bound_widget<edit_session<glm::vec3>>::draw_impl()
	{
		id_scope scope(&data);
		auto result = prefix_label(config.label);

		data.pre_edit();
		const glm::vec3 og = data.buffer();
		auto edit_result = item_result::query(ImGui::InputFloat3("", glm::value_ptr(data.buffer()), config.format, config.flags));
		result |= edit_result;
		if (imp::clamp(glm::value_ptr(data.buffer()), data.buffer().length(), config.min, config.max))
			result.modified = data.buffer() != og;
		data.post_edit(edit_result.state);

		result.modified |= prop::grid::check_property(std::make_unique<prop::simple_view<edit_session<glm::vec3>>>(data));
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

		result.modified |= prop::grid::check_property(std::make_unique<prop::simple_view<glm::vec4>>(data));
		return result;
	}

	item_result bound_widget<edit_session<glm::vec4>>::draw_impl()
	{
		id_scope scope(&data);
		auto result = prefix_label(config.label);

		data.pre_edit();
		const glm::vec4 og = data.buffer();
		auto edit_result = item_result::query(ImGui::InputFloat4("", glm::value_ptr(data.buffer()), config.format, config.flags));
		result |= edit_result;
		if (imp::clamp(glm::value_ptr(data.buffer()), data.buffer().length(), config.min, config.max))
			result.modified = data.buffer() != og;
		data.post_edit(edit_result.state);

		result.modified |= prop::grid::check_property(std::make_unique<prop::simple_view<edit_session<glm::vec4>>>(data));
		return result;
	}

	item_result bound_widget<std::string>::draw_impl()
	{
		id_scope scope(&data);
		auto result = prefix_label(config.label);

		result |= item_result::query(controls::input_text("", data, config.max_size, config.flags, config.callback, config.user_data));

		result.modified |= prop::grid::check_property(std::make_unique<prop::simple_view<std::string>>(data));
		return result;
	}

	item_result bound_widget<edit_session<std::string>>::draw_impl()
	{
		id_scope scope(&data);
		auto result = prefix_label(config.label);

		data.pre_edit();
		auto edit_result = item_result::query(controls::input_text("", data.buffer(), config.max_size, config.flags, config.callback, config.user_data));
		result |= edit_result;
		data.post_edit(edit_result.state);

		result.modified |= prop::grid::check_property(std::make_unique<prop::simple_view<edit_session<std::string>>>(data));
		return result;
	}

	item_result bound_widget<color4>::draw_impl()
	{
		id_scope scope(&data);
		auto result = prefix_label(config.label);

		result |= item_result::query(ImGui::ColorEdit4("", data.ptr(), config.flags));

		result.modified |= prop::grid::check_property(std::make_unique<prop::simple_view<color4>>(data));
		return result;
	}

	item_result bound_widget<edit_session<color4>>::draw_impl()
	{
		id_scope scope(&data);
		auto result = prefix_label(config.label);

		data.pre_edit();
		auto edit_result = item_result::query(ImGui::ColorEdit4("", data.buffer().ptr(), config.flags));
		result |= edit_result;
		data.post_edit(edit_result.state);

		result.modified |= prop::grid::check_property(std::make_unique<prop::simple_view<edit_session<color4>>>(data));
		return result;
	}
}
