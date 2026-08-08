#include "data_path.hpp"

namespace imtk
{
	datapath::datapath(datapath_view path)
		: _path(path._path.begin(), path._path.end())
	{
	}

	datapath datapath::operator/(step step) const
	{
		datapath p = *this;
		p._path.push_back(step);
		return p;
	}

	datapath& datapath::operator/=(step step)
	{
		_path.push_back(step);
		return *this;
	}

	datapath_view::datapath_view(const datapath& source)
		: _path(source._path)
	{
	}

	datapath_view& datapath_view::operator=(const datapath& source)
	{
		_path = source._path;
		return *this;
	}

	datapath datapath_view::clone() const
	{
		datapath source;
		source._path = std::vector(_path.begin(), _path.end());
		return source;
	}

	datapath datapath_view::operator/(datapath::step step) const
	{
		return std::move(clone() /= step);
	}

	bool datapath_view::empty() const
	{
		return _path.empty();
	}

	datapath::step datapath_view::step() const
	{
		return _path.front();
	}

	datapath_view datapath_view::next() const
	{
		datapath_view next;
		next._path = _path.subspan<1>();
		return next;
	}

	std::ostream& operator<<(std::ostream& os, datapath_view path)
	{
		os << "datapath(";

		for (size_t i = 0; i < path._path.size(); ++i)
		{
			os << (int)path._path[i];

			if (i + 1 < path._path.size())
				os << ", ";
		}

		return os << ")";
	}

	datapath datapath_link::node::compute_path() const
	{
		auto path = parent ? parent->compute_path() : datapath();
		if (step)
			path /= *step;
		return path;
	}

	datapath_link::datapath_link()
		: _node(std::make_unique<node>())
	{
	}

	datapath_link::datapath_link(datapath_link& parent, datapath::step step)
		: _node(std::make_unique<node>())
	{
		_node->parent = parent._node;
		_node->step = step;
	}

	std::optional<datapath::step> datapath_link::step() const
	{
		return _node->step;
	}

	void datapath_link::set_step(datapath::step step)
	{
		_node->step = step;
	}

	datapath_link datapath_link::share() const
	{
		datapath_link clone;
		clone._node = _node;
		return clone;
	}

	datapath datapath_link::compute_path() const
	{
		return _node->compute_path();
	}
}
