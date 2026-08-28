#pragma once

#include "imtk/datapath.hpp"
#include "imtk/logger.hpp"
#include "imtk/printer.hpp"

#include <imp/undo_history.hpp>

#include <sstream>

namespace imtk::field
{
	template<typename ty, typename printer = standard_printer<ty>>
	struct set_action : public imp::undo_action
	{
		datapath path;
		ty initial_value;
		ty final_value;

		set_action(datapath_view path, ty initial_value, ty final_value) :
			path(path), initial_value(std::move(initial_value)), final_value(std::move(final_value))
		{
		}

		bool forward() override
		{
			bool success = false;
			if (void* var = active_data_accessor::resolve(path, imp::erase_type<ty>()))
			{
				ty& ref = *static_cast<ty*>(var);
				ref = final_value;
				success = true;
			}

			std::stringstream ss;
			ss << "Redo action " << (success ? "success" : "fail") << ": [path=" << active_data_accessor::description(path);
			if constexpr (!std::is_void_v<printer>)
			{
				ss << ", from=";
				printer{}(ss, initial_value);
				ss << ", to=";
				printer{}(ss, final_value);
			}
			ss << "]";
			log(success ? log_level::success : log_level::error, ss.str());

			return success;
		}

		bool backward() override
		{
			bool success = false;
			if (void* var = active_data_accessor::resolve(path, imp::erase_type<ty>()))
			{
				ty& ref = *static_cast<ty*>(var);
				ref = initial_value;
				success = true;
			}

			std::stringstream ss;
			ss << "Undo action " << (success ? "success" : "fail") << ": [path=" << active_data_accessor::description(path);
			if constexpr (!std::is_void_v<printer>)
			{
				ss << ", from=";
				printer{}(ss, final_value);
				ss << ", to=";
				printer{}(ss, initial_value);
			}
			ss << "]";
			log(success ? log_level::success : log_level::error, ss.str());

			return success;
		}

		size_t empirical_size() const
		{
			return sizeof(*this);
		}
	};

	template<typename ty, typename printer = standard_printer<ty>>
	void push_set_action(datapath_view path, ty initial_value, ty final_value)
	{
		imp::undo_history::active_instance().push(std::make_unique<set_action<ty, printer>>(path, std::move(initial_value), std::move(final_value)));
	}
}
