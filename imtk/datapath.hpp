#pragma once

#include <ostream>
#include <optional>
#include <vector>
#include <span>

namespace imtk
{
	class datapath_view;

	class datapath
	{
	public:
		class step
		{
			int _v;

		public:
			constexpr explicit step(int v) : _v(v) {}

			constexpr bool operator==(const step& o) const { return _v == o._v; }
			constexpr bool operator==(int o) const { return _v == o; }
			constexpr operator int() const { return _v; }

			constexpr step& operator++() { ++_v; return *this; }
			constexpr step operator++(int) { step s = *this; ++_v; return s; }
			constexpr step operator+(int x) const { return step(_v + x); }
			constexpr step& operator--() { --_v; return *this; }
			constexpr step operator--(int) { step s = *this; --_v; return s; }
			constexpr step operator-(int x) const { return step(_v - x); }
		};

	private:
		friend class datapath_view;
		std::vector<step> _path;

	public:
		datapath() = default;
		datapath(datapath_view path);

		datapath operator/(step step) const;
		datapath& operator/=(step step);
	};

	class datapath_view
	{
		friend class datapath;
		std::span<const datapath::step> _path;

	public:
		datapath_view() = default;
		datapath_view(const datapath& source);
		datapath_view& operator=(const datapath& source);

		datapath clone() const;
		datapath operator/(datapath::step step) const;

		bool empty() const;
		datapath::step step() const;
		datapath_view next() const;

		friend std::ostream& operator<<(std::ostream& os, datapath_view path);
	};

	class datapath_link
	{
		struct node
		{
			friend class datapath_link;

			std::shared_ptr<node> parent;
			std::optional<datapath::step> step;

			datapath compute_path() const;
		};

		std::shared_ptr<node> _node;

	public:
		datapath_link();
		datapath_link(datapath_link& parent, datapath::step step);

		datapath_link(const datapath_link& o) = delete;
		datapath_link(datapath_link&& o) noexcept = default;

		datapath_link& operator=(const datapath_link& o) = delete;
		datapath_link& operator=(datapath_link&& o) noexcept = default;

		std::optional<datapath::step> step() const;
		void set_step(datapath::step step);
		datapath_link share() const;
		datapath compute_path() const;
	};
}

#define IMTK_DATAPATH_SUBLINK(subpath) imtk::datapath_link(this->link, subpath)
