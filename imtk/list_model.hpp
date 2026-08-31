#pragma once

#include <imp/bitmask.hpp>
#include <imp/modifiable.hpp>

namespace imtk
{
	enum class list_policy
	{
		none = 0,
		minimum_one = 1
	};

	IMP_IMPLEMENT_BITMASK(list_policy);

	enum class list_op_type
	{
		none_,
		append_,
		delete_,
		resize_,
		move_
	};

	class list_op
	{
		list_op_type _type = list_op_type::none_;
		bool _valid = true;
		size_t _index1 = 0;
		size_t _index2 = 0;

	public:
		static list_op make_append_op();
		static list_op make_delete_op(size_t index);
		static list_op make_resize_op(size_t old_size, size_t new_size);
		static list_op make_move_op(size_t src, size_t dst);

		list_op_type type() const;
		bool valid() const;

		size_t get_index() const;
		size_t get_src_index() const;
		size_t get_dst_index() const;
		size_t get_old_size() const;
		size_t get_new_size() const;

		bool update_index(list_policy policy, size_t& idx) const;
		bool update_index(list_policy policy, imp::modifiable<size_t>& idx) const;
		void update_op(list_policy policy, list_op& op) const;
	};
}
