#include "list_model.hpp"

namespace imtk
{
	list_op list_op::make_append_op(size_t old_size)
	{
		list_op op;
		op._type = list_op_type::append_;
		op._index1 = old_size;
		op._index2 = old_size + 1;
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

	size_t list_model::size() const
	{
		return _list_size;
	}

	// TODO only keep adapter init/sync, not size_t ones ?

	void list_model::init(size_t size)
	{
		_list_size = size;
		_index = 0;
	}

	void list_model::sync(size_t size)
	{
		_list_size = size;
		clamp();
	}

	void list_model::init(list_adapter& adapter)
	{
		init(adapter.size());
		enforce_policy(adapter);
	}

	void list_model::sync(list_adapter& adapter)
	{
		sync(adapter.size());
		enforce_policy(adapter);
	}
	
	void list_model::enforce_policy(list_adapter& adapter)
	{
		if (_list_size == 0 && imp::has_flag(policy, list_policy::minimum_one))
			apply(list_op::make_append_op(_list_size), adapter);
	}

	size_t list_model::index() const
	{
		return _index;
	}

	bool list_model::consume_index_modified()
	{
		return _index.consume_modified();
	}

	bool list_model::index_selected(size_t index) const
	{
		return _simul_selected.contains(index);
	}

	void list_model::on_select(size_t index, bool ctrl, bool shift)
	{
		if (ctrl)
		{
			if (_index != index)
			{
				if (!_simul_selected.contains(_index))
				{
					_simul_selected.insert(_index);
					_simul_selected_ordered.push_back(_index);
				}

				_index = index;
			}
			else
			{
				if (_simul_selected.contains(index))
				{
					_simul_selected.erase(index);
					_simul_selected_ordered.erase(std::find(_simul_selected_ordered.begin(), _simul_selected_ordered.end(), index));
				}

				if (!_simul_selected.empty())
				{
					_index = _simul_selected_ordered.back();
					_simul_selected_ordered.pop_back();
					_simul_selected.erase(_index);
				}
			}
		}
		else if (shift)
		{
			size_t min = std::min(index, _index.get());
			size_t max = std::max(index, _index.get());

			for (size_t i = min; i <= max; ++i)
			{
				if (i != index)
				{
					if (!_simul_selected.contains(i))
					{
						_simul_selected.insert(i);
						_simul_selected_ordered.push_back(i);
					}
				}
			}

			_index = index;
		}
		else
		{
			_simul_selected.clear();
			_simul_selected_ordered.clear();
			_index = index;
		}
	}

	void list_model::clamp()
	{
		if (_index >= _list_size)
			set_last();
	}

	void list_model::set_last()
	{
		_index = _list_size > 0 ? _list_size - 1 : 0;
	}

	void list_model::defer_append()
	{
		_ops.push_back(list_op::make_append_op(_list_size));
	}

	void list_model::defer_delete()
	{
		if (!_simul_selected.count(_index))
		{
			if (_index < _list_size)
				_ops.push_back(list_op::make_delete_op(_index));
		}

		for (size_t idx : _simul_selected_ordered)
		{
			if (idx < _list_size)
				_ops.push_back(list_op::make_delete_op(idx));
		}
	}
	
	void list_model::defer_resize(size_t size)
	{
		_ops.push_back(list_op::make_resize_op(_list_size, size));
	}

	void list_model::defer_clear()
	{
		defer_resize(0);
	}

	void list_model::defer_move(size_t src_index, size_t dst_index)
	{
		_ops.push_back(list_op::make_move_op(src_index, dst_index));
	}

	bool list_model::visit_deferred_ops(const std::function<void(const list_op&)>& fn)
	{
		bool any = false;

		for (auto it = _ops.begin(); it != _ops.end(); ++it)
		{
			if (!it->valid())
				continue;

			any = true;

			fn(*it);

			switch (it->type())
			{
			case list_op_type::append_:
				++_list_size;
				set_last();
				break;

			case list_op_type::delete_:
				--_list_size;
				break;

			case list_op_type::resize_:
				_list_size = it->get_new_size();
				break;
			}

			if (!it->update_index(policy, _index))
				clamp();

			std::unordered_set<size_t> keep_selected;
			for (auto ut = _simul_selected_ordered.begin(); ut != _simul_selected_ordered.end(); )
			{
				if (it->update_index(policy, *ut))
					keep_selected.insert(*ut++);
				else
					ut = _simul_selected_ordered.erase(ut);
			}
			_simul_selected = std::move(keep_selected);

			for (auto ut = std::next(it); ut != _ops.end(); ++ut)
				it->update_op(policy, *ut);
		}

		_ops.clear();
		return any;
	}

	void list_model::apply(const list_op& op, list_adapter& adapter)
	{
		switch (op.type())
		{
		case list_op_type::append_:
			++_list_size;
			set_last();
			break;

		case list_op_type::delete_:
			if (_list_size > 0)
				--_list_size;
			break;

		case list_op_type::resize_:
			_list_size = op.get_new_size();
			break;
		}

		adapter.apply(op);

		if (!op.update_index(policy, _index))
			clamp();

		enforce_policy(adapter);
	}

	// TODO merge common logic of consume_ops and visit_deferred_ops
	bool list_model::consume_ops(list_adapter& adapter)
	{
		bool any = false;

		for (auto it = _ops.begin(); it != _ops.end(); ++it)
		{
			if (!it->valid())
				continue;

			any = true;
			apply(*it, adapter);

			for (auto ut = std::next(it); ut != _ops.end(); )
			{
				it->update_op(policy, *ut);
				if (ut->valid())
					++ut;
				else
					ut = _ops.erase(ut);
			}
		}

		_ops.clear();
		return any;
	}

	void list_model::invoke(const imtk::list_op& op, list_adapter& adapter)
	{
		if (!_ops.empty())
			consume_ops(adapter);

		apply(op, adapter);
	}
}
