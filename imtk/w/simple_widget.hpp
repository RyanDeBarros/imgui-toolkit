#pragma once

#include "imtk/w/widget.hpp"
#include "imtk/types.hpp"

#include "external/glm.hpp"

#include <imp/potential.hpp>

#include <imgui.h>

namespace imtk::w
{
	template<typename ty>
	struct bound_widget;

	template<typename ty>
	struct simple_widget : public widget
	{
		ty value;
		bound_widget<ty> subwidget;

		using config_impl = typename bound_widget<ty>::config_impl;

		simple_widget(ty value = {}, config_impl config = {}) : value(value), subwidget(this->value, std::move(config)) {}

	protected:
		item_result draw_impl() override
		{
			return subwidget.draw();
		}

	public:
		config_impl& config()
		{
			return subwidget.config;
		}
	};

	template<>
	struct bound_widget<bool> : public widget
	{
		bool& data;

		struct config_impl
		{
			std::string label;
		} config;

		bound_widget(bool& data, config_impl config = {}) : data(data), config(std::move(config)) {}

	protected:
		item_result draw_impl() override;
	};

	template<>
	struct bound_widget<int> : public widget
	{
		int& data;
		
		struct config_impl
		{
			std::string label;
			imp::potential<int> min = imp::nullpotential;
			imp::potential<int> max = imp::nullpotential;
		} config;

		bound_widget(int& data, config_impl config = {}) : data(data), config(std::move(config)) {}

	protected:
		item_result draw_impl() override;
	};

	template<>
	struct bound_widget<float> : public widget
	{
		float& data;

		struct config_impl
		{
			std::string label;
			imp::potential<float> min = imp::nullpotential;
			imp::potential<float> max = imp::nullpotential;
		} config;

		bound_widget(float& data, config_impl config = {}) : data(data), config(std::move(config)) {}

	protected:
		item_result draw_impl() override;
	};

	template<>
	struct bound_widget<double> : public widget
	{
		double& data;
		
		struct config_impl
		{
			std::string label;
			imp::potential<double> min = imp::nullpotential;
			imp::potential<double> max = imp::nullpotential;
		} config;

		bound_widget(double& data, config_impl config = {}) : data(data), config(std::move(config)) {}

	protected:
		item_result draw_impl() override;
	};

	template<>
	struct bound_widget<glm::vec2> : public widget
	{
		glm::vec2& data;

		struct config_impl
		{
			std::string label;
			imp::potential<float> min = imp::nullpotential;
			imp::potential<float> max = imp::nullpotential;
		} config;

		bound_widget(glm::vec2& data, config_impl config = {}) : data(data), config(std::move(config)) {}

	protected:
		item_result draw_impl() override;
	};

	template<>
	struct bound_widget<glm::vec3> : public widget
	{
		glm::vec3& data;

		struct config_impl
		{
			std::string label;
			imp::potential<float> min = imp::nullpotential;
			imp::potential<float> max = imp::nullpotential;
		} config;

		bound_widget(glm::vec3& data, config_impl config = {}) : data(data), config(std::move(config)) {}

	protected:
		item_result draw_impl() override;
	};

	template<>
	struct bound_widget<glm::vec4> : public widget
	{
		glm::vec4& data;

		struct config_impl
		{
			std::string label;
			imp::potential<float> min = imp::nullpotential;
			imp::potential<float> max = imp::nullpotential;
		} config;

		bound_widget(glm::vec4& data, config_impl config = {}) : data(data), config(std::move(config)) {}

	protected:
		item_result draw_impl() override;
	};

	template<>
	struct bound_widget<std::string> : public widget
	{
		std::string& data;

		struct config_impl
		{
			std::string label;
		} config;

		bound_widget(std::string& data, config_impl config = {}) : data(data), config(std::move(config)) {}

	protected:
		item_result draw_impl() override;
	};

	template<>
	struct bound_widget<color4> : public widget
	{
		color4& data;

		struct config_impl
		{
			std::string label;
		} config;

		bound_widget(color4& data, config_impl config = {}) : data(data), config(std::move(config)) {}

	protected:
		item_result draw_impl() override;
	};
}
