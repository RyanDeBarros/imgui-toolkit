#pragma once

#include "imtk/prop/row_scope.hpp"
#include "imtk/desc/containers.hpp"

namespace imtk::prop
{
	template<typename ty, typename def_ty>
	class resettable_value : public iresettable
	{
		ty& _data;
		const def_ty& _def;

	public:
		resettable_value(ty& data, const def_ty& def)
			: _data(data), _def(def)
		{
		}

		bool out_of_sync() const override
		{
			return _data != _def;
		}

		void reset() override
		{
			_data = _def;
		}
	};

	template<typename ty, typename def_ty>
	class resettable_value<edit_session<ty>, def_ty> : public iresettable
	{
		edit_session<ty>& _data;
		const def_ty& _def;

	public:
		resettable_value(edit_session<ty>& data, const def_ty& def)
			: _data(data), _def(def)
		{
		}

		bool out_of_sync() const override
		{
			_data.pre_edit();
			return _data.buffer() != _def;
		}

		void reset() override
		{
			_data.publish_reset(_def);
		}
	};

	class resettable_row : public iresettable
	{
		std::vector<std::unique_ptr<iresettable>> _resetters;

	public:
		resettable_row(std::vector<std::unique_ptr<iresettable>> resetters)
			: _resetters(std::move(resetters))
		{
		}

		bool out_of_sync() const override
		{
			bool any = false;
			for (auto& r : _resetters)
				any |= r->out_of_sync();
			return any;
		}

		void reset() override
		{
			for (auto& r : _resetters)
				r->reset();
		}
	};

	template<typename ty>
	class resettable_vector_size : public iresettable
	{
		list_model& _model;
		desc::vector<ty>& _data;
		size_t _def_size;

	public:
		resettable_vector_size(list_model& model, desc::vector<ty>& data, const std::vector<ty>& def)
			: _model(model), _data(data), _def_size(def.size())
		{
		}

		bool out_of_sync() const override
		{
			return _data.size() != _def_size;
		}

		void reset() override
		{
			_model.defer_resize(_def_size);
		}
	};

	template<typename ty, typename def_ty>
	row_scope make_row_scope(std::string_view label, ty& data, const def_ty& def)
	{
		return row_scope(label, std::make_unique<resettable_value<ty, def_ty>>(data, def));
	}

	template<typename ty, typename def_ty>
	row_scope make_row_scope(std::string_view label, edit_session<ty>& data, const def_ty& def)
	{
		return row_scope(label, std::make_unique<resettable_value<edit_session<ty>, def_ty>>(data, def));
	}
}
