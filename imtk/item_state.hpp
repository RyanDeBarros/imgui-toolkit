#pragma once

namespace imtk
{
	class item_state
	{
		int _flags = 0;

	public:
		static item_state query();

		item_state operator|(const item_state&);
		item_state& operator|=(const item_state&);

		bool hovered() const;
		bool left_clicked() const;
		bool right_clicked() const;
		bool middle_clicked() const;
		bool clicked() const;
		bool focused() const;
		bool active() const;
		bool activated() const;
		bool deactivated() const;
		bool deactivated_after_edit() const;
		bool edited() const;
		bool visible() const;
		bool toggled_open() const;
		bool toggled_selection() const;
	};
}
