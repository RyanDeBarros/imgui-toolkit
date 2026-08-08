#include "tick.hpp"

#include <unordered_set>

namespace imtk
{
	static frame_number frame_counter = 0;

	static std::unordered_set<tick_processor*> tick_processors;

	void begin_frame()
	{
		++frame_counter;
	}

	void end_frame()
	{
		++frame_counter;

		for (tick_processor* o : tick_processors)
			o->on_last_process_frame();
	}

	frame_number frame()
	{
		return frame_counter;
	}

	tick_processor::tick_processor()
	{
		tick_processors.insert(this);
	}

	tick_processor::tick_processor(const tick_processor&)
	{
		tick_processors.insert(this);
	}

	tick_processor::tick_processor(tick_processor&&) noexcept
	{
		tick_processors.insert(this);
	}

	tick_processor::~tick_processor()
	{
		tick_processors.erase(this);
	}

	void tick_processor::processed_this_frame()
	{
		last_frame_processed = frame();
	}

	void tick_processor::check_for_last_processed_frame()
	{
		if (last_frame_processed + 1 == frame())
			on_last_process_frame();
	}
}
