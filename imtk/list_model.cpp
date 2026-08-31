#include "list_model.hpp"

namespace imtk
{
	list_op list_op::make_append_op()
	{
		list_op op;
		op._type = list_op_type::append_;
		return op;
	}

	list_op list_op::make_delete_op(size_t index)
	{
		list_op op;
		op._type = list_op_type::delete_;
		op._index1 = index;
		return op;
	}

	list_op list_op::make_resize_op(size_t old_size, size_t new_size)
	{
		list_op op;
		op._type = list_op_type::resize_;
		op._index1 = old_size;
		op._index2 = new_size;
		return op;
	}
	
	list_op list_op::make_move_op(size_t src, size_t dst)
	{
		list_op op;
		op._type = list_op_type::move_;
		op._index1 = src;
		op._index2 = dst;
		return op;
	}

	list_op_type list_op::type() const
	{
		return _type;
	}

	bool list_op::valid() const
	{
		return _valid;
	}

	size_t list_op::get_index() const
	{
		return _index1;
	}
	
	size_t list_op::get_src_index() const
	{
		return _index1;
	}
	
	size_t list_op::get_dst_index() const
	{
		return _index2;
	}
	
	size_t list_op::get_old_size() const
	{
		return _index1;
	}
	
	size_t list_op::get_new_size() const
	{
		return _index2;
	}

	bool list_op::update_index(list_policy policy, size_t& idx) const
	{
		imp::modifiable<size_t> m(idx);
		bool v = update_index(policy, m);
		idx = m;
		return v;
	}

	bool list_op::update_index(list_policy policy, imp::modifiable<size_t>& idx) const
	{
		switch (_type)
		{
		case list_op_type::append_:
			break;

		case list_op_type::delete_:
			if (idx == get_index())
				return false;

			if (idx > get_index())
				idx = idx - 1;

			break;

		case list_op_type::resize_:
			if (idx >= get_new_size())
				return false;

			break;

		case list_op_type::move_:
		{
			size_t min = std::min(get_src_index(), get_dst_index());
			size_t max = std::max(get_src_index(), get_dst_index());

			if (idx >= min && idx <= max)
			{
				if (idx == get_src_index())
					idx = get_dst_index();
				else if (get_src_index() < get_dst_index())
					idx = idx - 1;
				else
					idx = idx + 1;
			}

			break;
		}
		}

		return true;
	}

	void list_op::update_op(list_policy policy, list_op& op) const
	{
		if (op._type != list_op_type::append_ && op._type != list_op_type::resize_)
			op._valid = op._valid && update_index(policy, op._index1) && update_index(policy, op._index2);
	}
}
