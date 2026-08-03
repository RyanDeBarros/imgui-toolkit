#pragma once

namespace imtk
{
	template<typename e>
	struct enum_override
	{
		e require = {};
		e forbid = {};

		e eval(e defaults) const
		{
			const e valid_require = require & ~forbid;
			const e valid_forbid = forbid & ~require;

			defaults &= ~valid_forbid;
			defaults |= valid_require;
			return defaults;
		}
	};
}
