#pragma once

#include "imtk/datapath.hpp"
#include "imtk/printer.hpp"

#include "desc/vector_actions.hpp"
#include "field/list_actions.hpp"

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
		static list_op make_append_op(size_t old_size);
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

		template<typename ty, typename printer = standard_printer<ty>>
		void execute_desc_action(datapath path) const
		{
			switch (type())
			{
			case list_op_type::append_:
				desc::execute_vector_insert_action<ty, printer>(std::move(path), get_old_size());
				break;

			case list_op_type::delete_:
				desc::execute_vector_delete_action<ty, printer>(std::move(path), get_index());
				break;

			case list_op_type::resize_:
				if (get_old_size() != get_new_size())
					desc::execute_vector_resize_action<ty>(std::move(path), get_old_size(), get_new_size());
				break;

			case list_op_type::move_:
				if (get_src_index() != get_dst_index())
					desc::execute_vector_move_action<ty>(std::move(path), get_src_index(), get_dst_index());
				break;
			}
		}

		template<typename ty, typename printer = standard_printer<ty>>
		void execute_field_action(datapath path) const
		{
			switch (type())
			{
			case list_op_type::append_:
				field::execute_list_insert_action<ty, printer>(std::move(path), get_old_size());
				break;

			case list_op_type::delete_:
				field::execute_list_delete_action<ty, printer>(std::move(path), get_index());
				break;

			case list_op_type::resize_:
				if (get_old_size() != get_new_size())
					field::execute_list_resize_action<ty>(std::move(path), get_old_size(), get_new_size());
				break;

			case list_op_type::move_:
				if (get_src_index() != get_dst_index())
					field::execute_list_move_action<ty>(std::move(path), get_src_index(), get_dst_index());
				break;
			}
		}
	};
}
