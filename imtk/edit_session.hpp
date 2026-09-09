#pragma once

#include "imtk/item_result.hpp"
#include "imtk/tick.hpp"

#include <imp/instance_tracker.hpp>

#include <optional>

namespace imtk
{
	namespace internal
	{
		class iedit_session : public imp::instance_tracker<iedit_session>
		{
		protected:
			virtual void signal_publish_editing() = 0;

		public:
			static void signal_publish_editing_all()
			{
				for (auto it = instances().begin(); it != instances().end(); ++it)
					(*it)->signal_publish_editing();
			}
		};
	}

	template<typename ty>
	class edit_session : public tick_processor, public internal::iedit_session
	{
		bool _editing = false;
		
		ty& _truth;
		ty _buffer = ty();
		std::optional<ty> _published_from;

	public:
		edit_session(ty& truth)
			: tick_processor(tick_process_phase::submit_edit), _truth(truth)
		{
			_buffer = _truth;
		}

		edit_session(const edit_session&) = delete;
		edit_session(edit_session&&) = delete;

		edit_session& operator=(const edit_session& o)
		{
			if (this != &o)
			{
				_editing = o._editing;
				_buffer = o._buffer;
				_published_from = o._published_from;
			}

			return *this;
		}

		edit_session& operator=(edit_session&& o) noexcept
		{
			if (this != &o)
			{
				_editing = o._editing;
				_buffer = std::move(o._buffer);
				_published_from = std::move(o._published_from);
			}

			return *this;
		}

		const ty& truth() const
		{
			return _truth;
		}

		const ty& buffer() const
		{
			return _buffer;
		}

		ty& buffer()
		{
			return _buffer;
		}

		void pre_edit()
		{
			if (!_editing && _buffer != _truth)
				_buffer = _truth;

			processed_this_frame();
		}

		void post_edit(item_state state)
		{
			if (state.deactivated_after_edit())
				publish_confirm();

			if (state.activated())
			{
				_editing = true;
				_buffer = _truth;
			}
		}

	protected:
		void on_last_process_frame() override
		{
			if (_editing)
				publish_confirm();
		}

	public:
		std::optional<ty> consume_published_from()
		{
			std::optional<ty> pf = std::move(_published_from);
			_published_from.reset();
			return pf;
		}

		void publish_reset(ty to)
		{
			_buffer = std::move(to);
			publish_impl();
		}

		void publish_confirm()
		{
			_editing = false;
			publish_impl();
		}

		void cancel_editing()
		{
			_editing = false;
			_buffer = _truth;
			_published_from.reset();
		}

	protected:
		void signal_publish_editing() override
		{
			if (_editing)
				publish_impl();
		}

	private:
		void publish_impl()
		{
			if (_truth != _buffer)
			{
				_published_from = std::move(_truth);
				_truth = _buffer;
			}
		}
	};

	inline void signal_publish_editing_sessions()
	{
		internal::iedit_session::signal_publish_editing_all();
	}
}
