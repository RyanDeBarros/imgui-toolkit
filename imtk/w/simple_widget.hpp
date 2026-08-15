#pragma once

#include "imtk/w/widget.hpp"

#include "external/glm.hpp"

#include <imp/potential.hpp>

#include <imgui.h>

namespace imtk::w
{
	template<typename ty>
	struct simple_widget;

	template<>
	struct simple_widget<bool> : public widget
	{
		bool& data; // TODO v9.3 option to own data -> differentiate between simple_widget<bool> and simple_widget<bool&>

		struct config_impl
		{
			std::string label;
		} config;

		simple_widget(bool& data, config_impl config = {}) : data(data), config(std::move(config)) {}

	protected:
		item_result draw_impl() override;
	};

	template<>
	struct simple_widget<int> : public widget
	{
		int& data;
		
		struct config_impl
		{
			std::string label;
			imp::potential<int> min = imp::nullpotential;
			imp::potential<int> max = imp::nullpotential;
		} config;

		simple_widget(int& data, config_impl config = {}) : data(data), config(std::move(config)) {}

	protected:
		item_result draw_impl() override;
	};

	template<>
	struct simple_widget<float> : public widget
	{
		float& data;

		struct config_impl
		{
			std::string label;
			imp::potential<float> min = imp::nullpotential;
			imp::potential<float> max = imp::nullpotential;
		} config;

		simple_widget(float& data, config_impl config = {}) : data(data), config(std::move(config)) {}

	protected:
		item_result draw_impl() override;
	};

	template<>
	struct simple_widget<double> : public widget
	{
		double& data;
		
		struct config_impl
		{
			std::string label;
			imp::potential<double> min = imp::nullpotential;
			imp::potential<double> max = imp::nullpotential;
		} config;

		simple_widget(double& data, config_impl config = {}) : data(data), config(std::move(config)) {}

	protected:
		item_result draw_impl() override;
	};

	template<>
	struct simple_widget<glm::vec2> : public widget
	{
		glm::vec2& data;

		struct config_impl
		{
			std::string label;
			imp::potential<float> min = imp::nullpotential;
			imp::potential<float> max = imp::nullpotential;
		} config;

		simple_widget(glm::vec2& data, config_impl config = {}) : data(data), config(std::move(config)) {}

	protected:
		item_result draw_impl() override;
	};

	template<>
	struct simple_widget<glm::vec3> : public widget
	{
		glm::vec3& data;

		struct config_impl
		{
			std::string label;
			imp::potential<float> min = imp::nullpotential;
			imp::potential<float> max = imp::nullpotential;
		} config;

		simple_widget(glm::vec3& data, config_impl config = {}) : data(data), config(std::move(config)) {}

	protected:
		item_result draw_impl() override;
	};

	template<>
	struct simple_widget<glm::vec4> : public widget
	{
		glm::vec4& data;

		struct config_impl
		{
			std::string label;
			imp::potential<float> min = imp::nullpotential;
			imp::potential<float> max = imp::nullpotential;
		} config;

		simple_widget(glm::vec4& data, config_impl config = {}) : data(data), config(std::move(config)) {}

	protected:
		item_result draw_impl() override;
	};

	template<>
	struct simple_widget<std::string> : public widget
	{
		std::string& data;

		struct config_impl
		{
			std::string label;
		} config;

		simple_widget(std::string& data, config_impl config = {}) : data(data), config(std::move(config)) {}

	protected:
		item_result draw_impl() override;
	};
}
