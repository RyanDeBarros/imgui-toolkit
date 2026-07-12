#pragma once

#include "imtk/errors.hpp"

namespace imtk
{
	template<typename derived>
	struct instance_guard
	{
	private:
		static inline derived* _instance = nullptr;

	public:
		instance_guard()
		{
			if (_instance)
				throw error(error_code::existing_active_instance);

			_instance = static_cast<derived*>(this);
		}
		
		instance_guard(const instance_guard&) = delete;
		instance_guard(instance_guard&&) = delete;
		
		~instance_guard()
		{
			if (_instance == static_cast<derived*>(this))
				_instance = nullptr;
		}

		static derived& instance()
		{
			if (_instance)
				return *_instance;
			else
				throw error(error_code::no_active_instance);
		}
	};
}
