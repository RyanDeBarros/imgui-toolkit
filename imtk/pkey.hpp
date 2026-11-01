#pragma once

#ifdef IMTK_KEY_TYPE
#include IMTK_KEY_DECL_PATH
#endif

#include "external/toml.hpp"

#include <string>
#include <string_view>

namespace imtk
{
	struct pkey
	{
		unsigned long long k = 0;

		constexpr pkey(unsigned long long key) : k(key) {}
		constexpr operator unsigned long long() const { return k; }

		constexpr static pkey null() { return pkey(0); }

		constexpr bool operator==(const pkey&) const = default;
		constexpr bool operator!=(const pkey&) const = default;

		// TODO note in documentation about setting this macro in cmake
#ifdef IMTK_KEY_TYPE
		constexpr pkey(IMTK_KEY_TYPE k)
            : k(static_cast<unsigned long long>(k))
        {
        }
		
        constexpr operator IMTK_KEY_TYPE() const
        {
            return static_cast<IMTK_KEY_TYPE>(k);
        }
#endif
	};

	extern void set_pkey_encoder(std::string(*encoder)(pkey));
	extern void set_pkey_decoder(pkey(*decoder)(std::string_view));

	extern std::string encode_pkey(pkey key);
	extern pkey decode_pkey(std::string_view key);

	extern toml_node toml_get(toml_node node, pkey key);
}
