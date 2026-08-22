#pragma once

#ifdef IMTK_KEY_TYPE
#include IMTK_KEY_DECL_PATH
#endif

#include <string>
#include <string_view>

namespace imtk
{
	struct key
	{
		unsigned long long k = 0;

		constexpr key(unsigned long long key) : k(key) {}
		constexpr operator unsigned long long() const { return k; }

		constexpr static key null() { return key(0); }

		constexpr bool operator==(const key&) const = default;
		constexpr bool operator!=(const key&) const = default;

		// TODO note in documentation about setting this macro in cmake
#ifdef IMTK_KEY_TYPE
		constexpr key(IMTK_KEY_TYPE k) : k(static_cast<unsigned long long>(k)) {}
		constexpr operator IMTK_KEY_TYPE() const { return static_cast<IMTK_KEY_TYPE>(k); }
#endif
	};

	extern void set_key_encoder(std::string(*encoder)(key));
	extern void set_key_decoder(key(*decoder)(std::string_view));

	extern std::string encode_key(key key);
	extern key decode_key(std::string_view key);
}
