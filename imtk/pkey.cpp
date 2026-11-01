#include "pkey.hpp"

namespace imtk
{
	static std::string(*pkey_encoder)(pkey) = nullptr;
	static pkey(*pkey_decoder)(std::string_view) = nullptr;

	void set_pkey_encoder(std::string(*encoder)(pkey))
	{
		pkey_encoder = encoder;
	}

	void set_pkey_decoder(pkey(*decoder)(std::string_view))
	{
		pkey_decoder = decoder;
	}

	std::string encode_pkey(pkey key)
	{
		return pkey_encoder(key);
	}

    pkey decode_pkey(std::string_view key)
	{
		return pkey_decoder(key);
	}

	toml_node toml_get(toml_node node, pkey key)
	{
		if (key != pkey::null())
			return node[encode_pkey(key)];
		else
			return node;
	}
}
