#include "id_scope.hpp"

#include <imgui.h>

namespace imtk
{
	id_scope::id_scope(const void* ptr_id)
	{
		push(ptr_id);
	}

	id_scope::id_scope(int int_id)
	{
		push(int_id);
	}

	id_scope::id_scope(const char* str_id)
	{
		push(str_id);
	}

	id_scope::id_scope(id_scope&& o) noexcept
		: _depth(o._depth)
	{
		o._depth = 0;
	}

	id_scope::~id_scope()
	{
		pop_all();
	}

	id_scope& id_scope::operator=(id_scope&& o) noexcept
	{
		if (this != &o)
		{
			pop_all();
			_depth = o._depth;
			o._depth = 0;
		}
		return *this;
	}

	id_scope::operator bool() const
	{
		return _depth > 0;
	}

	id_scope& id_scope::push(const void* ptr_id)
	{
		ImGui::PushID(ptr_id);
		++_depth;
		return *this;
	}

	id_scope& id_scope::push(int int_id)
	{
		ImGui::PushID(int_id);
		++_depth;
		return *this;
	}

	id_scope& id_scope::push(const char* str_id)
	{
		ImGui::PushID(str_id);
		++_depth;
		return *this;
	}

	void id_scope::pop()
	{
		ImGui::PopID();
		--_depth;
	}

	void id_scope::pop_all()
	{
		while (_depth > 0)
			pop();
	}
}
