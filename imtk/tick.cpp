#include "tick.hpp"

#include <unordered_map>
#include <unordered_set>

namespace imtk
{
	static frame_number frame_counter = 0;

	static std::unordered_map<tick_process_phase, std::unordered_set<tick_processor*>> tick_processors;

	void begin_frame()
	{
		++frame_counter;
	}

	void end_frame()
	{
		++frame_counter;

		tick_processor::process_last_frames(tick_process_phase::submit_edit);
		tick_processor::process_last_frames(tick_process_phase::check_undo);
		tick_processor::process_last_frames(tick_process_phase::query_dirty);
	}

	frame_number frame()
	{
		return frame_counter;
	}

	tick_processor::tick_processor(tick_process_phase phase)
		: _phase(phase)
	{
		tick_processors[_phase].insert(this);
	}

	tick_processor::tick_processor(const tick_processor& o)
		: _phase(o._phase), _last_frame_processed(o._last_frame_processed)
	{
		tick_processors[_phase].insert(this);
	}

	tick_processor::tick_processor(tick_processor&& o) noexcept
		: _phase(o._phase), _last_frame_processed(o._last_frame_processed)
	{
		tick_processors[_phase].insert(this);
	}

	tick_processor::~tick_processor()
	{
		tick_processors[_phase].erase(this);
	}

	tick_processor& tick_processor::operator=(const tick_processor& o)
	{
		if (this != &o)
		{
			_last_frame_processed = o._last_frame_processed;
			set_phase(o._phase);
		}

		return *this;
	}
	
	tick_processor& tick_processor::operator=(tick_processor&& o) noexcept
	{
		if (this != &o)
		{
			_last_frame_processed = o._last_frame_processed;
			set_phase(o._phase);
		}

		return *this;
	}

	void tick_processor::processed_this_frame()
	{
		_last_frame_processed = frame();
	}

	void tick_processor::process_last_frames(tick_process_phase phase)
	{
		for (tick_processor* o : tick_processors[phase])
			o->check_for_last_processed_frame();
	}

	void tick_processor::check_for_last_processed_frame()
	{
		if (_last_frame_processed + 1 == frame())
			on_last_process_frame();
	}
	void tick_processor::set_phase(tick_process_phase phase)
	{
		if (phase != _phase)
		{
			tick_processors[_phase].erase(this);
			_phase = phase;
			tick_processors[_phase].insert(this);
		}
	}
}
