#pragma once

#include "imtk/errors.hpp"

#include <stack>

namespace imtk
{
	template<typename derived>
	struct instance_stack
	{
	private:
		static inline std::stack<derived*> _instances = nullptr;

	public:
		instance_stack()
		{
			_instances.push(static_cast<derived*>(this));
		}

		instance_stack(const instance_stack&) = delete;
		instance_stack(instance_stack&&) = delete;

		~instance_stack()
		{
			_instances.pop();
		}

		static derived& instance()
		{
			if (!_instances.empty())
				return *_instances.top();
			else
				throw error(error_code::no_active_instance);
		}
	};
}
