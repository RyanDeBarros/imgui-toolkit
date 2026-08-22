#include "key.hpp"

namespace imtk
{
	static std::string(*key_encoder)(key) = nullptr;
	static key(*key_decoder)(std::string_view) = nullptr;

	void set_key_encoder(std::string(*encoder)(key))
	{
		key_encoder = encoder;
	}

	void set_key_decoder(key(*decoder)(std::string_view))
	{
		key_decoder = decoder;
	}

	std::string encode_key(key key)
	{
		return key_encoder(key);
	}

	key decode_key(std::string_view key)
	{
		return key_decoder(key);
	}
}
