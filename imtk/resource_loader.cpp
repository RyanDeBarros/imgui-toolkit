#include "resource_loader.hpp"

#include <imp/hetero.hpp>

namespace imtk::res
{
	std::vector<texture> icon_textures;
	static imp::hetero::string_umap<icon_id> icon_texture_lut;

	icon_id::operator bool() const
	{
		return id > 0;
	}

	icon_id load_icon(std::string_view filepath)
	{
		auto it = icon_texture_lut.find(filepath);
		if (it != icon_texture_lut.end())
			return it->second;
		else
		{
			size_t index = icon_textures.size();
			icon_textures.push_back(texture::load_generic(filepath, GL_LINEAR, GL_LINEAR, 1.f, true));
			return icon_texture_lut.emplace(std::string(filepath), icon_id{ .id = index + 1 }).first->second;
		}
	}

	texture icon_texture(icon_id id)
	{
		if (id.id == 0 || id.id > icon_textures.size())
			return texture();
		else
			return icon_textures[id.id - 1];
	}
}
