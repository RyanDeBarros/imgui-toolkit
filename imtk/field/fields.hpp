#pragma once

#include "imtk/datapath.hpp"
#include "imtk/edit_session.hpp"
#include "imtk/key.hpp"

#include "imtk/field/set_action.hpp"

#include "imtk/prop/common_resetters.hpp"
#include "imtk/prop/form.hpp"
#include "imtk/prop/property_grid.hpp"
#include "imtk/prop/row_scope.hpp"

#include "imtk/w/dynamic_list.hpp"
#include "imtk/w/common_widgets.hpp"
#include "imtk/w/simple_widget.hpp"
#include "imtk/w/widget_row.hpp"

#include <imp/group.hpp>

// TODO separate into separate files - not all headers above are required for primitive fields

namespace imtk::field
{
	template<typename ty>
	struct primitive_fld : public tick_processor
	{
		datapath_link link;
		ty def;
		ty value;
		edit_session<ty> edit;
		key key_;
		const char* label; // TODO v9.3 use label_registry for all const char* labels -> use in widgets as well over std::string?

		primitive_fld(datapath_link link, ty def, key key, const char* label)
			: tick_processor(tick_process_phase::check_undo), link(std::move(link)), def(def), value(def), edit(value), key_(key), label(label)
		{
		}

		primitive_fld(primitive_fld&& o) noexcept
			: tick_processor(std::move(o)), link(std::move(o.link)), def(std::move(o.def)), value(std::move(o.value)), edit(value), key_(o.key_), label(o.label)
		{
		}

		primitive_fld& operator=(const primitive_fld&) = delete;

		primitive_fld& operator=(primitive_fld&& o) noexcept = default;

		void copy_data(const primitive_fld& o)
		{
			edit.publish_reset(o.value);
		}

		bool query_dirty(const primitive_fld& disk) const
		{
			return value != disk.value;
		}

		void load(toml_node node)
		{
			ty val = def;
			serializer<ty>{}.load(val, toml_get(node, key_));
			edit.publish_reset(std::move(val));
		}

		void dump(toml::table& table) const
		{
			if (key_ != key::null())
				table.insert_or_assign(encode_key(key_), serializer<ty>{}.dump(edit.truth()));
		}

		void dump(toml::array& array) const
		{
			array.push_back(serializer<ty>{}.dump(edit.truth()));
		}

		void* resolve(datapath_view path, imp::type_erasure type)
		{
			return path.empty() ? imp::resolve_type(type, &value) : nullptr;
		}

	protected:
		void on_last_process_frame() override
		{
			check_undo_action();
		}

	public:
		void check_undo_action()
		{
			if (auto original = edit.consume_published_from())
				push_set_action(link.compute_path(), std::move(*original), edit.truth());
		}

		void draw()
		{
			if (auto row = prop::make_row_scope(label, edit, def))
				prop::value::add_component(std::make_unique<w::bound_widget<edit_session<ty>>>(edit));

			check_undo_action();
		}
	};

	struct bool_fld
	{
		datapath_link link;
		bool def;
		bool value;
		key key_;
		const char* label;

		bool_fld(datapath_link link, bool def, key key, const char* label)
			: link(std::move(link)), def(def), value(def), key_(key), label(label)
		{
		}

		void copy_data(const bool_fld& o)
		{
			value = o.value;
		}

		bool query_dirty(const bool_fld& disk) const
		{
			return value != disk.value;
		}

		void load(toml_node node)
		{
			value = def;
			serializer<bool>{}.load(value, toml_get(node, key_));
		}

		void dump(toml::table& table) const
		{
			if (key_ != key::null())
				table.insert_or_assign(encode_key(key_), serializer<bool>{}.dump(value));
		}

		void dump(toml::array& array) const
		{
			array.push_back(serializer<bool>{}.dump(value));
		}

		void* resolve(datapath_view path, imp::type_erasure type)
		{
			return path.empty() ? imp::resolve_type(type, &value) : nullptr;
		}

		void draw()
		{
			const bool og = value;

			if (auto row = prop::make_row_scope(label, value, def))
				prop::value::add_component(std::make_unique<w::bound_widget<bool>>(value));

			if (og != value)
				push_set_action(link.compute_path(), og, value);
		}
	};

	template<typename ty, typename u, imp::potential<u> _min, imp::potential<u> _max>
	struct range_fld : public primitive_fld<ty>
	{
		using super = primitive_fld<ty>;

		inline static const imp::potential<u> min = _min;
		inline static const imp::potential<u> max = _max;

		using super::super;

		void draw()
		{
			if (auto row = prop::make_row_scope(this->label, this->edit, this->def))
			{
				auto widget = std::make_unique<w::bound_widget<edit_session<ty>>>(this->edit);
				widget->subwidget.config.min = min;
				widget->subwidget.config.max = max;
				prop::value::add_component(std::move(widget));
			}

			this->check_undo_action();
		}
	};

	template<imp::potential<int> min, imp::potential<int> max>
	using int_fld = range_fld<int, int, min, max>;

	template<imp::potential<float> min, imp::potential<float> max>
	using float_fld = range_fld<float, float, min, max>;

	template<imp::potential<double> min, imp::potential<double> max>
	using double_fld = range_fld<double, double, min, max>;

	template<typename e> requires (std::is_enum_v<e>)
		struct enum_fld
	{
		datapath_link link;
		e def;
		e value;
		key key_;
		const char* label;

		enum_fld(datapath_link link, e def, key key, const char* label)
			: link(std::move(link)), def(def), value(def), key_(key), label(label)
		{
		}

		void copy_data(const enum_fld& o)
		{
			value = o.value;
		}

		bool query_dirty(const enum_fld& disk) const
		{
			return value != disk.value;
		}

		void load(toml_node node)
		{
			value = def;
			serializer<e>{}.load(value, toml_get(node, key_));
		}

		void dump(toml::table& table) const
		{
			if (key_ != key::null())
				table.insert_or_assign(encode_key(key_), serializer<e>{}.dump(value));
		}

		void dump(toml::array& array) const
		{
			array.push_back(serializer<e>{}.dump(value));
		}

		void* resolve(datapath_view path, imp::type_erasure type)
		{
			return path.empty() ? imp::resolve_type(type, &value) : nullptr;
		}

		void draw()
		{
			const e og = value;
			int int_value = static_cast<int>(value);
			const int int_default = static_cast<int>(def);

			if (auto row = prop::make_row_scope(label, int_value, int_default))
				prop::value::add_component(std::make_unique<w::combo_widget>(int_value, combo_names()));

			value = static_cast<e>(int_value);
			if (og != value)
				push_set_action(link.compute_path(), og, value);
		}

		static label_span_registry::handle combo_names();
	};

	using string_fld = primitive_fld<std::string>;
	using color4_fld = primitive_fld<color4>;

	template<typename field, size_t n>
	struct array_fld
	{
		datapath_link link;
		std::array<field, n> fields;
		key key_;
		const char* label;

		template<typename ty>
		array_fld(datapath_link link, std::array<ty, n> def, key key, const char* label)
			: link(std::move(link))
			, fields(init_fields(std::make_index_sequence<n>{}, this->link, std::move(def), nullptr))
			, key_(key)
			, label(label)
		{
		}

		template<typename ty>
		array_fld(datapath_link link, std::array<ty, n> def, key key, const char* label, const char* (&sublabels)[n])
			: link(std::move(link))
			, fields(init_fields(std::make_index_sequence<n>{}, this->link, std::move(def), sublabels))
			, key_(key)
			, label(label)
		{
		}

	private:
		template<typename ty, size_t... i>
		static auto init_fields(std::index_sequence<i...>, datapath_link& link, std::array<ty, n> def, const char** sublabels)
		{
			return std::array<field, n>{ field(datapath_link(link, datapath::step(i)), std::move(def[i]), key::null(), sublabels ? sublabels[i] : "")... };
		}

	public:
		void copy_data(const array_fld& o)
		{
			for (size_t i = 0; i < n; ++i)
				fields[i].copy_data(o.fields[i]);
		}

		bool query_dirty(const array_fld& disk) const
		{
			for (size_t i = 0; i < n; ++i)
			{
				if (fields[i].query_dirty(disk.fields[i]))
					return true;
			}

			return false;
		}

		void load(toml_node node)
		{
			if (auto arr = toml_get(node, key_).as_array())
			{
				for (size_t i = 0; i < n; ++i)
				{
					if (i < arr->size())
						fields[i].load(toml_node(arr->get(i)));
					else
						fields[i].load({});
				}
			}
		}

		void dump(toml::table& table) const
		{
			if (key_ != key::null())
			{
				toml::array arr;
				arr.reserve(n);
				for (size_t i = 0; i < n; ++i)
					fields[i].dump(arr);
				table.insert_or_assign(encode_key(key_), std::move(arr));
			}
		}

		void dump(toml::array& array) const
		{
			if (key_ != key::null())
			{
				toml::array arr;
				arr.reserve(n);
				for (size_t i = 0; i < n; ++i)
					fields[i].dump(arr);
				array.push_back(std::move(arr));
			}
		}

		void* resolve(datapath_view path, imp::type_erasure type)
		{
			if (path.empty())
				return imp::resolve_type(type, this);

			int index = path.step();
			if (index >= 0 && index < n)
				return fields[index].resolve(path.next(), type);
			else
				return nullptr;
		}

		void draw()
		{
			// TODO v9.3 subviews of each field
			//prop::view_generator generator = [this]() {
			//	auto view = std::make_unique<prop::view_list>();
			//	view->subviews.reserve(n);
			//	for (size_t i = 0; i < n; ++i)
			//		view->subviews.push_back(std::make_unique<prop::simple_view<std::string>>(data.buffer()[i]));
			//	return view;
			//};
			//if (auto subform = prop::subform(label, generator))

			if (auto subform = prop::subform(label))
			{
				for (auto& field : fields)
					field.draw();
			}
		}
	};

	template<size_t n>
	struct bool_array_fld
	{
		datapath_link link;
		std::array<bool, n> def;
		std::array<bool, n> value;
		key key_;
		const char* label;
		label_span_registry::handle sublabels = {};
		bool inline_checkboxes;

		bool_array_fld(datapath_link link, std::array<bool, n> def, key key, const char* label, const char* (&sublabels)[n], bool inline_checkboxes)
			: link(std::move(link))
			, def(def)
			, value(def)
			, key_(key)
			, label(label)
			, sublabels(label_span_registry::intern(std::span<const char* const>(sublabels, n)))
			, inline_checkboxes(inline_checkboxes)
		{
		}

		void copy_data(const bool_array_fld& o)
		{
			value = o.value;
		}

		bool query_dirty(const bool_array_fld& disk) const
		{
			return value != disk.value;
		}

		void load(toml_node node)
		{
			serializer<std::array<bool, n>>{}.load(value, toml_get(node, key_));
		}

		void dump(toml::table& table) const
		{
			if (key_ != key::null())
				table.insert_or_assign(encode_key(key_), serializer<std::array<bool, n>>{}.dump(value));
		}

		void dump(toml::array& array) const
		{
			array.push_back(serializer<std::array<bool, n>>{}.dump(value));
		}

		void* resolve(datapath_view path, imp::type_erasure type)
		{
			if (path.empty())
				return imp::resolve_type(type, &this->value);

			int index = path.step();
			if (index >= 0 && index < n)
			{
				path = path.next();
				return path.empty() ? imp::resolve_type(type, &this->value[index]) : nullptr;
			}
			else
				return nullptr;
		}

		void draw()
		{
			const std::array<bool, n> og = value;

			imp::group<bool> data_group(value);
			imp::group<const bool> def_group(def);

			if (auto row = prop::make_row_scope(label, data_group, def_group))
			{
				std::vector<std::unique_ptr<w::widget>> widgets;

				for (size_t i = 0; i < n; ++i)
					widgets.push_back(w::unique_bound_widget(value[i], { .label = label_span_registry::string(sublabels, i) }));

				if (inline_checkboxes)
					prop::value::add_component(std::make_unique<w::widget_row>(std::move(widgets)));
				else
					prop::value::add_component(std::make_unique<w::sequence>(std::move(widgets)));
			}

			if (og != value)
				push_set_action(link.compute_path(), og, value);
		}
	};

	template<typename ty>
	struct vector_fld : public primitive_fld<std::vector<ty>>
	{
		dynamic_list widget;

		using primitive_fld<std::vector<ty>>::primitive_fld;
	};

	using string_vector_fld = vector_fld<std::string>;

	template<typename e>
	struct disjoint_enum_fld
	{
		datapath_link link;
		e def;
		int index_;
		int def_index;
		key key_;
		const char* label;
		const e* values;
		label_span_registry::handle names = {};
		size_t count;

		template<size_t n>
		disjoint_enum_fld(datapath_link link, e def, key key, const char* label, const e(&values)[n], const char* (&names)[n])
			: link(std::move(link)), def(def), key_(key), label(label), values(values), names(label_span_registry::intern(std::span<const char*>(names, n))), count(n)
		{
			set_value(def);
			def_index = index(def);
		}

		void copy_data(const disjoint_enum_fld& o)
		{
			index_ = o.index_;
		}

		bool query_dirty(const disjoint_enum_fld<e>& disk) const
		{
			return index_ != disk.index_;
		}

		void draw()
		{
			const auto initial = index_;

			if (auto row = prop::make_row_scope(label, index_, def_index))
				prop::value::add_component(std::make_unique<w::combo_widget>(index_, names));

			if (initial != index_)
				push_set_action(link.compute_path(), initial, index_);
		}

		void load(toml_node node)
		{
			index_ = index(static_cast<e>(toml_get(node, key_).value_or(def)));
		}

		void dump(toml::table& table) const
		{
			table.insert_or_assign(encode_key(key_), value());
		}

		void dump(toml::array& array) const
		{
			array.push_back(value());
		}

		e value() const
		{
			return values[index_];
		}

		void set_value(const e val)
		{
			index_ = index(val);
		}

		int index(const e val) const
		{
			for (size_t i = 0; i < count; ++i)
			{
				if (val == values[i])
					return i;
			}

			return -1;
		}

		void* resolve(datapath_view path, imp::type_erasure type)
		{
			return path.empty() ? imp::resolve_type(type, &index_) : nullptr;
		}
	};

	template<typename ty, imp::potential<ty> _min, imp::potential<ty> _max>
	struct optional_range_fld : public tick_processor
	{
		using Self = optional_range_fld<ty, _min, _max>;
		inline static const imp::potential<ty> min = _min;
		inline static const imp::potential<ty> max = _max;

		datapath_link link;
		imp::potential<ty> def;
		imp::potential<ty> value;
		edit_session<imp::potential<ty>> edit;
		key value_key;
		key enable_key;
		const char* label;

		optional_range_fld(datapath_link link, imp::potential<ty> def, key value_key, key enable_key, const char* label)
			: tick_processor(tick_process_phase::check_undo), link(std::move(link)), def(def), value(def), edit(value), value_key(value_key), enable_key(enable_key), label(label)
		{
		}

		optional_range_fld(optional_range_fld&& o) noexcept
			: tick_processor(std::move(o)), link(std::move(o.link)), def(std::move(o.def)), value(std::move(o.value)), edit(value), value_key(o.value_key), enable_key(o.enable_key), label(o.label)
		{
		}

		optional_range_fld& operator=(const optional_range_fld&) = delete;
		optional_range_fld& operator=(optional_range_fld&&) noexcept = default;

		void copy_data(const optional_range_fld& o)
		{
			edit.publish_reset(o.value);
		}

		bool query_dirty(const optional_range_fld& disk) const
		{
			return value != disk.value;
		}

		void load(toml_node node)
		{
			imp::potential<ty> val = def;

			if (enable_key != key::null() && value_key != key::null())
			{
				serializer<ty>{}.load(val.value, node[encode_key(value_key)]);
				serializer<bool>{}.load(val.has_value, node[encode_key(enable_key)]);
			}

			edit.publish_reset(std::move(val));
		}

		void dump(toml::table& table) const
		{
			if (enable_key != key::null() && value_key != key::null())
			{
				table.insert_or_assign(encode_key(enable_key), serializer<bool>{}.dump(edit.truth().has_value));
				table.insert_or_assign(encode_key(value_key), serializer<ty>{}.dump(edit.truth().value));
			}
		}

		void draw()
		{
			if (auto row = prop::make_row_scope(label, edit, def))
			{
				auto widget = std::make_unique<w::bound_widget<edit_session<imp::potential<ty>>>>(edit);
				widget->subwidget.value.config.min = min;
				widget->subwidget.value.config.max = max;
				prop::value::add_component(std::move(widget));
			}

			check_undo_action();
		}

		void check_undo_action()
		{
			if (auto original = edit.consume_published_from())
				push_set_action(link.compute_path(), std::move(*original), edit.truth());
		}

		void* resolve(datapath_view path, imp::type_erasure type)
		{
			return path.empty() ? imp::resolve_type(type, &value) : nullptr;
		}

	protected:
		void on_last_process_frame() override
		{
			check_undo_action();
		}
	};

	template<imp::potential<int> min, imp::potential<int> max>
	using optional_int_fld = optional_range_fld<int, min, max>;

	template<imp::potential<float> min, imp::potential<float> max>
	using optional_float_fld = optional_range_fld<float, min, max>;

	template<imp::potential<double> min, imp::potential<double> max>
	using optional_double_fld = optional_range_fld<double, min, max>;

	template<typename ty, imp::potential<ty> _min, imp::potential<ty> _max>
	struct compact_optional_range_fld : public tick_processor
	{
		inline static const imp::potential<ty> min = _min;
		inline static const imp::potential<ty> max = _max;

		datapath_link link;
		imp::potential<ty> def;
		imp::potential<ty> value;
		edit_session<imp::potential<ty>> edit;
		ty nullopt;
		key key_;
		const char* label;

		compact_optional_range_fld(datapath_link link, imp::potential<ty> def, ty nullopt, key key, const char* label)
			: tick_processor(tick_process_phase::check_undo), link(std::move(link)), def(def), value(def), edit(value), nullopt(nullopt), key_(key), label(label)
		{
		}

		compact_optional_range_fld(compact_optional_range_fld&& o) noexcept
			: tick_processor(std::move(o)), link(std::move(o.link)), def(std::move(o.def)), value(std::move(o.value)), edit(value), nullopt(o.nullopt), key_(o.key_), label(o.label)
		{
		}

		compact_optional_range_fld& operator=(const compact_optional_range_fld&) = delete;
		compact_optional_range_fld& operator=(compact_optional_range_fld&&) noexcept = default;

		void copy_data(const compact_optional_range_fld& o)
		{
			edit.publish_reset(o.value);
		}

		bool query_dirty(const compact_optional_range_fld& disk) const
		{
			return value != disk.value;
		}

		void draw()
		{
			if (auto row = prop::make_row_scope(label, edit, def))
			{
				auto widget = std::make_unique<w::bound_widget<edit_session<imp::potential<ty>>>>(edit);
				widget->subwidget.value.config.min = min;
				widget->subwidget.value.config.max = max;
				prop::value::add_component(std::move(widget));
			}

			check_undo_action();
		}

		void check_undo_action()
		{
			if (auto original = edit.consume_published_from())
				push_set_action(link.compute_path(), std::move(*original), edit.truth());
		}

		void load(toml_node node)
		{
			imp::potential<ty> val = def;

			if (key_ != key::null())
			{
				ty temp = def.value;
				if (serializer<ty>{}.load(temp, node[encode_key(key_)]))
				{
					val.has_value = temp != nullopt;
					if (val.has_value)
						val.value = temp;
				}
				else
					val.has_value = false;
			}

			edit.publish_reset(std::move(val));
		}

		void dump(toml::table& table) const
		{
			if (key_ != key::null())
				table.insert_or_assign(encode_key(key_), serializer<ty>{}.dump(edit.truth().has_value ? edit.truth().value : nullopt));
		}

		void dump(toml::array& array) const
		{
			array.push_back(serializer<ty>{}.dump(edit.truth().has_value ? edit.truth().value : nullopt));
		}

		void* resolve(datapath_view path, imp::type_erasure type)
		{
			return path.empty() ? imp::resolve_type(type, &value) : nullptr;
		}

		void on_last_process_frame() override
		{
			check_undo_action();
		}
	};

	template<imp::potential<int> min, imp::potential<int> max>
	using compact_optional_int_fld = compact_optional_range_fld<int, min, max>;

	template<imp::potential<float> min, imp::potential<float> max>
	using compact_optional_float_fld = compact_optional_range_fld<float, min, max>;

	template<imp::potential<float> min, imp::potential<float> max, glm::length_t L>
	using vec_fld = range_fld<glm::vec<L, float>, float, min, max>;

	template<imp::potential<float> min, imp::potential<float> max>
	using vec2_fld = vec_fld<min, max, 2>;

	template<imp::potential<float> min, imp::potential<float> max>
	using vec3_fld = vec_fld<min, max, 3>;

	template<imp::potential<float> min, imp::potential<float> max>
	using vec4_fld = vec_fld<min, max, 4>;

	template<typename e, size_t count_>
	struct bitset_fld
	{
		datapath_link link;
		bool def_flags[count_];
		bool value_flags[count_];
		e def;
		e value;
		key key_;
		const char* label;
		const e* values;
		label_span_registry::handle names = {};
		bool inline_checkboxes;

		static const inline size_t count = count_;

		bitset_fld(datapath_link link, e def, key key, const char* label, const e(&values)[count_], const char* (&names)[count_], bool inline_checkboxes)
			: link(std::move(link))
			, def(def)
			, value(def)
			, key_(key)
			, label(label)
			, values(values)
			, names(label_span_registry::intern(std::span<const char* const>(names, count)))
			, inline_checkboxes(inline_checkboxes)
		{
			set_flags();
		}

		void copy_data(const bitset_fld& o)
		{
			value = o.value;
		}

		void draw(const bool(&disabled)[count])
		{
			return draw(static_cast<const bool*>(disabled));
		}

		void draw()
		{
			return draw(nullptr);
		}

	private:
		void draw(const bool* disabled)
		{
			const auto initial = value;
			set_flags();

			imp::group<bool> data_group(value_flags, count);
			imp::group<const bool> def_group(def_flags, count);

			if (auto row = prop::make_row_scope(label, data_group, def_group))
			{
				std::vector<std::unique_ptr<w::widget>> widgets;

				for (size_t i = 0; i < count; ++i)
				{
					widgets.push_back(std::make_unique<w::disabler>(
						w::unique_bound_widget(value_flags[i], { .label = label_span_registry::string(names, i) }),
						disabled && disabled[i]
					));
				}

				if (inline_checkboxes)
					prop::value::add_component(std::make_unique<w::widget_row>(std::move(widgets)));
				else
					prop::value::add_component(std::make_unique<w::sequence>(std::move(widgets)));
			}

			set_enum();
			if (initial != value)
				push_set_action(link.compute_path(), initial, value);
		}

		void set_flags()
		{
			for (size_t i = 0; i < count; ++i)
			{
				value_flags[i] = static_cast<bool>(value & values[i]);
				def_flags[i] = static_cast<bool>(def & values[i]);
			}
		}

		void set_enum()
		{
			for (size_t i = 0; i < count; ++i)
			{
				if (value_flags[i])
					value |= values[i];
				else
					value &= ~values[i];
			}
		}

	public:
		void load(toml_node node)
		{
			value = def;
			serializer<e>{}.load(value, toml_get(node, key_));
		}

		void dump(toml::table& table) const
		{
			if (key_ != key::null())
				table.insert_or_assign(encode_key(key_), serializer<e>{}.dump(value));
		}

		void dump(toml::array& array) const
		{
			array.push_back(serializer<e>{}.dump(value));
		}

		void* resolve(datapath_view path, imp::type_erasure type)
		{
			return path.empty() ? imp::resolve_type(type, &value) : nullptr;
		}

		bool query_dirty(const bitset_fld<e, count>& disk) const
		{
			return value != disk.value;
		}
	};
}
