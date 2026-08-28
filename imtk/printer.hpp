#pragma once

#include <array>
#include <ostream>
#include <vector>

#include <glm/glm.hpp>

namespace imtk
{
	template<typename ty>
	struct standard_printer
	{
		void operator()(std::ostream& os, const ty& obj) const
		{
			os << obj;
		}
	};

	template<typename ty>
	struct standard_printer<std::vector<ty>>
	{
		void operator()(std::ostream& os, const std::vector<ty>& obj) const
		{
			os << "std::vector[ ";

			for (auto it = obj.begin(); it != obj.end(); ++it)
			{
				os << *it;

				if (std::next(it) != obj.end())
					os << ", ";
			}

			os << " ]";
		}
	};

	template<typename ty, size_t N>
	struct standard_printer<std::array<ty, N>>
	{
		void operator()(std::ostream& os, const std::array<ty, N>& obj)
		{
			os << "std::array[ ";

			for (size_t i = 0; i < N; ++i)
			{
				os << obj[i];

				if (i + 1 < N)
					os << ", ";
			}

			os << " ]";
		}
	};

	template<glm::length_t L>
	struct standard_printer<glm::vec<L, float>>
	{
		void operator()(std::ostream& os, const glm::vec<L, float> obj)
		{
			os << "glm::vec" << L << "(";

			for (glm::length_t i = 0; i < L; ++i)
			{
				os << obj[i];

				if (i + 1 < L)
					os << ", ";
			}

			os << ")";
		}
	};
}
