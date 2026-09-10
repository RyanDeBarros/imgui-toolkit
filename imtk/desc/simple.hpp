#pragma once

#include "imtk/key.hpp"
#include "imtk/serializer.hpp"

namespace imtk::desc
{
	template<typename d>
	struct simple
	{
		d desc;
		imtk::key key;

		simple(imtk::key key)
			: key(key)
		{
		}

		void load(imtk::toml_node node)
		{
			desc.load(node[imtk::encode_key(key)]);
		}

		void dump(toml::table& table)
		{
			toml::table subtable;
			desc.dump(subtable);
			table.insert_or_assign(imtk::encode_key(key), std::move(subtable));
		}

		const d& operator*() const
		{
			return desc;
		}

		d& operator*()
		{
			return desc;
		}

		const d* operator->() const
		{
			return &desc;
		}

		d* operator->()
		{
			return &desc;
		}

		const d* value_ptr() const
		{
			return &desc;
		}

		d* value_ptr()
		{
			return &desc;
		}
	};

	template<typename d>
	struct simple_array
	{
		std::vector<d> descs;
		imtk::key key;

		simple_array(imtk::key key)
			: key(key)
		{
		}

		void load(imtk::toml_node node)
		{
			descs.clear();
			if (auto array = node[imtk::encode_key(key)].as_array())
			{
				descs.resize(array->size());
				for (size_t i = 0; i < descs.size(); ++i)
					descs[i].load(imtk::toml_node(*array->get(i)));
			}
		}

		void dump(toml::table& table)
		{
			toml::array array;
			for (size_t i = 0; i < descs.size(); ++i)
			{
				toml::table subtable;
				descs[i].dump(subtable);
				array.push_back(std::move(subtable));
			}
			table.insert_or_assign(imtk::encode_key(key), std::move(array));
		}

		const d& operator[](size_t i) const
		{
			return descs[i];
		}

		d& operator[](size_t i)
		{
			return descs[i];
		}

		auto begin() const noexcept
		{
			return descs.begin();
		}

		auto end() const noexcept
		{
			return descs.end();
		}

		auto begin() noexcept
		{
			return descs.begin();
		}

		auto end() noexcept
		{
			return descs.end();
		}

		auto cbegin() const noexcept
		{
			return descs.cbegin();
		}

		auto cend() const noexcept
		{
			return descs.cend();
		}
	};
}
