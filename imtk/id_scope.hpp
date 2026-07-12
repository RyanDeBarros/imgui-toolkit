#pragma once

namespace imtk
{
	class id_scope
	{
		int _depth = 0;

	public:
		id_scope() = default;
		id_scope(const void* ptr_id);
		id_scope(int int_id);
		id_scope(const char* str_id);

		id_scope(const id_scope&) = delete;
		id_scope(id_scope&& o) noexcept;
		~id_scope();
		id_scope& operator=(id_scope&& o) noexcept;

		operator bool() const;

		id_scope& push(const void* ptr_id);
		id_scope& push(int int_id);
		id_scope& push(const char* str_id);

		void pop();
		void pop_all();
	};
}
