#pragma once

#include "imtk/datapath.hpp"
#include "imtk/key.hpp"
#include "imtk/list_model.hpp"

#include "imtk/prop/property_grid.hpp"

namespace imtk::desc
{
	template<typename d>
	class vector
	{
		std::vector<d> _vector;

	public:
		datapath_link link;
		key key_;

		vector(key key, datapath_link link)
			: link(std::move(link)), key_(key)
		{
		}

		d& push_back()
		{
			_vector.push_back(d(datapath_link(link, datapath::step(_vector.size()))));
			return _vector.back();
		}

		void insert(size_t i, d element)
		{
			for (auto it = _vector.begin() + i; it != _vector.end(); ++it)
				it->link.set_step(*it->link.step() + 1);

			element.link.parent_to(link, datapath::step(i));
			_vector.insert(_vector.begin() + i, std::move(element));
		}

		void remove(size_t i)
		{
			_vector.erase(_vector.begin() + i);
			for (auto it = _vector.begin() + i; it != _vector.end(); ++it)
				it->link.set_step(*it->link.step() - 1);
		}

		void clear()
		{
			_vector.clear();
		}

		size_t size() const
		{
			return _vector.size();
		}

		void resize(size_t new_size)
		{
			if (new_size < _vector.size())
				_vector.erase(_vector.begin() + new_size, _vector.end());
			else if (new_size > _vector.size())
			{
				for (size_t i = _vector.size(); i < new_size; ++i)
					_vector.push_back(d(datapath_link(link, datapath::step(i))));
			}
		}

		bool empty() const
		{
			return _vector.empty();
		}

		d& back()
		{
			return _vector.back();
		}

		const d& back() const
		{
			return _vector.back();
		}

		d& operator[](size_t i)
		{
			return _vector[i];
		}

		const d& operator[](size_t i) const
		{
			return _vector[i];
		}

		auto begin() const
		{
			return _vector.begin();
		}

		auto begin()
		{
			return _vector.begin();
		}

		auto end() const
		{
			return _vector.end();
		}

		auto end()
		{
			return _vector.end();
		}

		void* resolve(datapath_view path, imp::type_erasure type)
		{
			if (path.empty())
				return imp::resolve_type(type, this);

			int index = path.step();
			if (index >= 0 && index < _vector.size())
				return _vector[index].resolve(path.next(), type);
			else
				return nullptr;
		}

		void describe(std::ostream& os, datapath_view path) const
		{
			if (path.empty())
				os << "<error>";
			else
			{
				int index = path.step();
				if (index >= 0 && index < _vector.size())
				{
					path = path.next();
					if (path.empty())
						os << index;
					else
					{
						os << index << ".";
						_vector[index].describe(os, path);
					}
				}
				else
					os << "<error>";
			}
		}

		bool query_dirty(const vector<d>& disk) const
		{
			if (_vector.size() != disk._vector.size())
				return true;

			for (size_t i = 0; i < _vector.size(); ++i)
			{
				if (_vector[i].query_dirty(disk._vector[i]))
					return true;
			}

			return false;
		}

		void copy_data(const vector<d>& o)
		{
			resize(o.size());
			for (size_t i = 0; i < _vector.size(); ++i)
				_vector[i].copy_data(o._vector[i]);
		}

		toml_node subnode(toml_node node) const
		{
			return node[encode_key(key_)];
		}

		template<typename value>
		void dump_into(toml::table& table, value&& value_) const
		{
			table.insert_or_assign(encode_key(key_), std::forward<value>(value_));
		}

		struct list_op_adapter : public ilist_op_adapter
		{
			vector<d>& desc;

			list_op_adapter(vector<d>& desc)
				: desc(desc)
			{
			}

			void apply(const list_op& op) const override
			{
				op.execute_desc_action<d>(desc.link.compute_path());
			}
		};

		void consume_ops(list_model& model)
		{
			if (model.consume_ops(list_op_adapter(*this)))
				prop::grid::mark_dirty();
		}
	};
}
