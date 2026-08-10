#pragma once

#include "external/toml.hpp"
#include "external/glm.hpp"

#include <concepts>
#include <set>
#include <vector>

namespace imtk
{
	template<typename ty>
	struct serializer;

	template<>
	struct serializer<bool>
	{
		bool load(bool& obj, toml_node node) const
		{
			if (auto v = node.value<bool>())
			{
				obj = *v;
				return true;
			}
			else
				return false;
		}

		bool dump(const bool obj) const
		{
			return obj;
		}
	};

	template<>
	struct serializer<int>
	{
		bool load(int& obj, toml_node node) const
		{
			if (auto v = node.value<int64_t>())
			{
				obj = static_cast<int>(*v);
				return true;
			}
			else
				return false;
		}

		int64_t dump(const int obj) const
		{
			return static_cast<int64_t>(obj);
		}
	};

	template<>
	struct serializer<int64_t>
	{
		bool load(int64_t& obj, toml_node node) const
		{
			if (auto v = node.value<int64_t>())
			{
				obj = *v;
				return true;
			}
			else
				return false;
		}

		int64_t dump(const int64_t obj) const
		{
			return obj;
		}
	};

	template<>
	struct serializer<unsigned int>
	{
		bool load(unsigned int& obj, toml_node node) const
		{
			if (auto v = node.value<int64_t>())
			{
				obj = static_cast<unsigned int>(*v);
				return true;
			}
			else
				return false;
		}

		int64_t dump(const unsigned int obj) const
		{
			return static_cast<int64_t>(obj);
		}
	};

	template<>
	struct serializer<unsigned char>
	{
		bool load(unsigned char& obj, toml_node node) const
		{
			if (auto v = node.value<int64_t>())
			{
				obj = static_cast<unsigned char>(*v);
				return true;
			}
			else
				return false;
		}

		int64_t dump(const unsigned char obj) const
		{
			return static_cast<int64_t>(obj);
		}
	};

	template<typename e> requires std::is_enum_v<e>
	struct serializer<e>
	{
		bool load(e& obj, toml_node node) const
		{
			if (auto v = node.value<int64_t>())
			{
				obj = static_cast<e>(*v);
				return true;
			}
			else
				return false;
		}

		int64_t dump(const e obj) const
		{
			return static_cast<int64_t>(obj);
		}
	};

	template<>
	struct serializer<float>
	{
		bool load(float& obj, toml_node node) const
		{
			if (auto v = node.value<double>())
			{
				obj = static_cast<float>(*v);
				return true;
			}
			else
				return false;
		}

		double dump(const float obj) const
		{
			return static_cast<double>(obj);
		}
	};

	template<>
	struct serializer<double>
	{
		bool load(double& obj, toml_node node) const
		{
			if (auto v = node.value<double>())
			{
				obj = *v;
				return true;
			}
			else
				return false;
		}

		double dump(const double obj) const
		{
			return obj;
		}
	};

	template<>
	struct serializer<std::string>
	{
		bool load(std::string& obj, toml_node node) const
		{
			if (auto v = node.value<std::string>())
			{
				obj = std::move(*v);
				return true;
			}
			else
				return false;
		}

		std::string dump(const std::string& obj) const
		{
			return obj;
		}
	};

	template<typename ty, glm::length_t length>
	struct serializer<glm::vec<length, ty>>
	{
		bool load(glm::vec<length, ty>& obj, toml_node node) const
		{
			if (auto arr = node.as_array())
			{
				bool fully_loaded = true;
				for (glm::length_t i = 0; i < glm::min(static_cast<glm::length_t>(arr->size()), length); ++i)
					fully_loaded &= serializer<ty>{}.load(obj[i], toml_node(*arr->get(i)));
				return fully_loaded;
			}
			else
				return false;
		}

		toml::array dump(const glm::vec<length, ty> obj) const
		{
			toml::array arr;
			arr.reserve(length);
			for (glm::length_t i = 0; i < length; ++i)
				arr.push_back(serializer<ty>{}.dump(obj[i]));
			return arr;
		}
	};

	template<typename ty, size_t n>
	struct serializer<std::array<ty, n>>
	{
		bool load(std::array<ty, n>& obj, toml_node node) const
		{
			if (auto arr = node.as_array())
			{
				bool fully_loaded = true;
				for (size_t i = 0; i < std::min(arr->size(), n); ++i)
					fully_loaded &= serializer<ty>{}.load(obj[i], toml_node(*arr->get(i)));
				return fully_loaded;
			}
			else
				return false;
		}

		toml::array dump(const std::array<ty, n>& obj) const
		{
			toml::array arr;
			arr.reserve(n);
			for (size_t i = 0; i < n; ++i)
				arr.push_back(serializer<ty>{}.dump(obj[i]));
			return arr;
		}
	};

	template<typename T>
	struct serializer<std::vector<T>>
	{
		bool load(std::vector<T>& obj, toml_node node) const
		{
			if (auto arr = node.as_array())
			{
				obj.clear();
				bool fully_loaded = true;
				for (size_t i = 0; i < arr->size(); ++i)
				{
					T el{};
					if (serializer<T>{}.load(el, toml_node(*arr->get(i))))
						obj.push_back(std::move(el));
					else
						fully_loaded = false;
				}
				return fully_loaded;
			}
			else
				return false;
		}

		toml::array dump(const std::vector<T>& obj) const
		{
			toml::array arr;
			arr.reserve(obj.size());
			for (const T& el : obj)
				arr.push_back(serializer<T>{}.dump(el));
			return arr;
		}
	};

	template<typename T>
	struct serializer<std::set<T>>
	{
		bool load(std::set<T>& obj, toml_node node) const
		{
			if (auto arr = node.as_array())
			{
				obj.clear();
				bool fully_loaded = true;
				for (size_t i = 0; i < arr->size(); ++i)
				{
					T el{};
					if (serializer<T>{}.load(el, toml_node(*arr->get(i))))
						obj.insert(std::move(el));
					else
						fully_loaded = false;
				}
				return fully_loaded;
			}
			else
				return false;
		}

		toml::array dump(const std::set<T>& obj) const
		{
			toml::array arr;
			arr.reserve(obj.size());
			for (const T& el : obj)
				arr.push_back(serializer<T>{}.dump(el));
			return arr;
		}
	};
}
