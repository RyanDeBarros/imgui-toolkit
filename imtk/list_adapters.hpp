#pragma once

#include "imtk/list_model.hpp"
#include "imtk/printer.hpp"

#include "imtk/desc/vector.hpp"

#include <imp/counter.hpp>

#include <memory>

namespace imtk
{
	template<typename ty>
	struct vector_sized : public isized
	{
		const desc::vector<ty>& v;

		vector_sized(const desc::vector<ty>& vec) : v(vec) {}

		size_t size() const override
		{
			return v.size();
		}
	};

	template<typename ty, typename printer = standard_printer<ty>>
	struct vector_op_adapter : public ilist_op_adapter
	{
		const desc::vector<ty>& v;

		vector_op_adapter(const desc::vector<ty>& vec) : v(vec) {}

		void apply(const list_op& op) const override
		{
			op.execute_desc_action<ty, printer>(v.link.compute_path());
		}
	};

	template<typename ty>
	vector_op_adapter<ty, standard_printer<ty>> make_vector_op_adapter(const desc::vector<ty>& vector)
	{
		return vector_op_adapter<ty, standard_printer<ty>>(vector);
	}

	template<typename printer, typename ty>
	vector_op_adapter<ty, printer> make_vector_op_adapter(const desc::vector<ty>& vector)
	{
		return vector_op_adapter<ty, printer>(vector);
	}

	template<typename ty>
	std::unique_ptr<vector_op_adapter<ty, standard_printer<ty>>> make_unique_vector_op_adapter(const desc::vector<ty>& vector)
	{
		return std::make_unique<vector_op_adapter<ty, standard_printer<ty>>>(vector);
	}

	template<typename printer, typename ty>
	std::unique_ptr<vector_op_adapter<ty, printer>> make_unique_vector_op_adapter(const desc::vector<ty>& vector)
	{
		return std::make_unique<vector_op_adapter<ty, printer>>(vector);
	}

	template<typename ty>
	list_adapter make_vector_adapter(const desc::vector<ty>& vector)
	{
		return {
			.sized = std::make_unique<vector_sized<ty>>(vector),
			.ops = make_unique_vector_op_adapter<ty>(vector)
		};
	}

	template<typename printer, typename ty>
	list_adapter make_vector_adapter(const desc::vector<ty>& vector)
	{
		return {
			.sized = std::make_unique<vector_sized<ty>>(vector),
			.ops = make_unique_vector_op_adapter<printer, ty>(vector)
		};
	}

	struct list_callback_adapter : public ilist_op_adapter
	{
		std::unique_ptr<ilist_op_adapter> primary;
		std::function<void(const list_op&)> callback;

		list_callback_adapter(std::unique_ptr<ilist_op_adapter>&& primary, std::function<void(const list_op&)> callback)
			: primary(std::move(primary)), callback(std::move(callback)) {}

		void apply(const list_op& op) const override
		{
			callback(op);
			primary->apply(op);
		}
	};

	template<typename ty, typename getter_ty, typename hash, typename equals>
	std::function<void(list_op)> make_counter_callback(imp::counter<ty, hash, equals>& counter, getter_ty getter)
	{
		return [&counter, getter = std::move(getter)](list_op op) {
			switch (op.type())
			{
			case list_op_type::append_:
				counter.increment(ty{});
				break;

			case list_op_type::delete_:
				counter.decrement(getter(op.get_index()));
				break;

			case list_op_type::resize_:
				if (op.get_new_size() == 0)
					counter.clear();
				else
				{
					for (size_t i = op.get_new_size(); i < op.get_old_size(); ++i)
						counter.decrement(getter(i));

					if (op.get_old_size() < op.get_new_size())
						counter.increment(ty{}, op.get_new_size() - op.get_old_size());
				}
				break;
			}
		};
	}

	template<typename ty, typename getter_ty>
	std::function<void(list_op)> make_counter_callback(imp::counter<ty, std::hash<ty>, std::equal_to<ty>>& counter, getter_ty getter)
	{
		return make_counter_callback<ty, getter_ty, std::hash<ty>, std::equal_to<ty>>(counter, std::move(getter));
	}
}
