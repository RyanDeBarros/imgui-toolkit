#pragma once

#include "imtk/edit_session.hpp"
#include "imtk/list_model.hpp"
#include "imtk/prop/property_grid.hpp"

#include <memory>
#include <string_view>
#include <vector>

namespace imtk::prop
{
	struct iresettable
	{
		virtual ~iresettable() = default;

		virtual bool out_of_sync() const = 0;
		virtual void reset() = 0;
	};

	class row_scope
	{
		bool _valid;
		std::unique_ptr<iresettable> _resetter;

	public:
		row_scope(std::string_view label, std::unique_ptr<iresettable> resetter);
		row_scope(row_scope&& o) noexcept;
		~row_scope();

		row_scope(const row_scope&) = delete;
		row_scope& operator=(row_scope&&) = delete;

		operator bool() const;
	};

	class multi_row_scope
	{
		bool _valid;
		std::vector<std::unique_ptr<iresettable>> _resetters;

	public:
		multi_row_scope(std::string_view label, std::vector<std::unique_ptr<iresettable>> resetters);
		multi_row_scope(multi_row_scope&& o) noexcept;
		~multi_row_scope();

		multi_row_scope(const multi_row_scope&) = delete;
		multi_row_scope& operator=(multi_row_scope&&) = delete;

		operator bool() const;
	};

	template<typename ty>
	class vector_row_scope
	{
		edit_session<std::vector<ty>>& _data;
		const std::vector<ty>& _def;
		list_model& _model;

	public:
		vector_row_scope(std::string_view label, edit_session<std::vector<ty>>& data, const std::vector<ty>& def, list_model& model)
			: _data(data), _def(def), _model(model)
		{
			key::set_label(label);
			_data.pre_edit();
			if (_data.buffer().size() != _def.size())
				reset::button(0);

			for (size_t i = 0; i < _data.buffer().size(); ++i)
			{
				if (i < _def.size())
				{
					if (_data.buffer()[i] != _def[i])
						reset::button(1 + i);
				}
				else
				{
					if (_data.buffer()[i] != ty{})
						reset::button(1 + i);
				}
			}
		}
		
		~vector_row_scope()
		{
			row::submit();

			_data.post_edit(row::get_draw_result().state);
			if (reset::activated(0))
				_model.defer_resize(_def.size());

			std::vector<std::string> reset = _data.buffer();
			bool publish = false;

			for (size_t i = 0; i < _data.buffer().size(); ++i)
			{
				if (reset::activated(1 + i))
				{
					reset[i] = i < _def.size() ? _def[i] : ty{};
					publish = true;
				}
			}

			if (publish)
				_data.publish_reset(std::move(reset));
		}

		vector_row_scope(const vector_row_scope&) = delete;
		vector_row_scope(vector_row_scope&&) = delete;

		operator bool() const
		{
			return true;
		}
	};
}
