#include "texture.hpp"

#include "imtk/errors.hpp"
#include "imtk/tick.hpp"

#include "external/nsvg.hpp"
#include "external/stb.hpp"

#include <imp/hash.hpp>

#include <filesystem>
#include <fstream>
#include <unordered_map>

namespace imtk
{
	struct raster_texture_ctor
	{
		std::filesystem::path filepath;
		GLenum min_filter, mag_filter;
		bool generate_mipmaps;

		bool operator==(const raster_texture_ctor&) const = default;
	};

	struct gif_texture_ctor
	{
		std::filesystem::path filepath;
		GLenum min_filter, mag_filter;
		bool generate_mipmaps;

		bool operator==(const gif_texture_ctor&) const = default;
	};

	struct svg_texture_ctor
	{
		std::filesystem::path filepath;
		GLenum min_filter, mag_filter;
		float scale;
		bool generate_mipmaps;

		bool operator==(const svg_texture_ctor&) const = default;
	};

	struct texture_ctor_hash
	{
		size_t operator()(const raster_texture_ctor& tc) const
		{
			return imp::hasher()
				.with(tc.filepath)
				.with(tc.min_filter)
				.with(tc.mag_filter)
				.with(tc.generate_mipmaps);
		}

		size_t operator()(const gif_texture_ctor& tc) const
		{
			return imp::hasher()
				.with(tc.filepath)
				.with(tc.min_filter)
				.with(tc.mag_filter)
				.with(tc.generate_mipmaps);
		}

		size_t operator()(const svg_texture_ctor& tc) const
		{
			return imp::hasher()
				.with(tc.filepath)
				.with(tc.min_filter)
				.with(tc.mag_filter)
				.with(tc.scale)
				.with(tc.generate_mipmaps);
		}
	};

	texture_id::texture_id()
	{
		glGenTextures(1, &_id);
	}

	texture_id::texture_id(texture_id&& other) noexcept
		: _id(other._id)
	{
		other._id = 0;
	}

	texture_id::~texture_id()
	{
		glDeleteTextures(1, &_id);
	}

	texture_id& texture_id::operator=(texture_id&& other) noexcept
	{
		if (this != &other)
		{
			glDeleteTextures(1, &_id);
			_id = other._id;
			other._id = 0;
		}
		return *this;
	}

	GLuint texture_id::id() const
	{
		return _id;
	}

	static GLenum internal_format(int channels)
	{
		return channels == 1 ? GL_R8
			: channels == 2 ? GL_RG8
			: channels == 3 ? GL_RGB8
			: GL_RGBA8;
	}

	static GLenum format(int channels)
	{
		return channels == 1 ? GL_RED
			: channels == 2 ? GL_RG
			: channels == 3 ? GL_RGB
			: GL_RGBA;
	}

	raster_texture::raster_texture(const std::string_view filepath, GLenum min_filter, GLenum mag_filter, bool generate_mipmaps)
	{
		int channels;
		unsigned char* data = stbi_load(filepath.data(), &width, &height, &channels, 0);
		if (!data || width <= 0 || height <= 0 || channels <= 0)
		{
			stbi_image_free(data);
			throw error(error_code::load_texture, "Cannot load raster image from file: " + std::string(filepath));
		}

		glBindTexture(GL_TEXTURE_2D, id.id());
		glTexImage2D(GL_TEXTURE_2D, 0, internal_format(channels), width, height, 0, format(channels), GL_UNSIGNED_BYTE, data);
		if (generate_mipmaps)
			glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
		glBindTexture(GL_TEXTURE_2D, 0);

		stbi_image_free(data);
	}

	static std::unordered_map<raster_texture_ctor, std::weak_ptr<raster_texture>, texture_ctor_hash> raster_textures;

	std::shared_ptr<raster_texture> raster_texture::load(const std::string_view filepath, std::optional<GLenum> min_filter, std::optional<GLenum> mag_filter, bool generate_mipmaps)
	{
		raster_texture_ctor ctor{
			.filepath = filepath,
			.min_filter = min_filter ? *min_filter : GL_NEAREST,
			.mag_filter = mag_filter ? *mag_filter : GL_NEAREST,
			.generate_mipmaps = generate_mipmaps
		};

		auto it = raster_textures.find(ctor);
		if (it != raster_textures.end() && !it->second.expired())
			return it->second.lock();

		std::shared_ptr<raster_texture> sp(new raster_texture(filepath, ctor.min_filter, ctor.mag_filter, ctor.generate_mipmaps));
		raster_textures[ctor] = sp;
		return sp;
	}

	GLuint raster_texture::get_id() const
	{
		return id.id();
	}

	float raster_texture::get_width() const
	{
		return width;
	}

	float raster_texture::get_height() const
	{
		return height;
	}

	gif_texture::gif_texture(const std::string_view filepath, GLenum min_filter, GLenum mag_filter, bool generate_mipmaps)
	{
		std::ifstream file(filepath.data(), std::ios::binary | std::ios::ate);
		if (!file.is_open() || file.fail())
			throw error(error_code::load_texture, "Cannot open file for reading: " + std::string(filepath));

		std::streamsize size = file.tellg();
		if (size <= 0)
			throw error(error_code::load_texture, "File has bad size (bytes): " + std::string(filepath));

		file.seekg(0, std::ios::beg);

		std::vector<unsigned char> pixels(size);
		file.read(reinterpret_cast<char*>(pixels.data()), size);

		int channels;
		int* delay_arr;
		int frames;
		unsigned char* data = stbi_load_gif_from_memory(pixels.data(), static_cast<int>(pixels.size()), &delay_arr, &width, &height, &frames, &channels, 0);
		if (!data || !delay_arr || frames <= 0 || width <= 0 || height <= 0 || channels <= 0)
		{
			stbi_image_free(delay_arr);
			stbi_image_free(data);
			throw error(error_code::load_texture, "Cannot load gif from file: " + std::string(filepath));
		}

		const int area = sizeof(unsigned char) * width * height * channels;
		delays.resize(frames);
		for (int i = 0; i < frames; ++i)
			delays[i] = 0.01f * delay_arr[i];

		ids.resize(frames);
		for (size_t i = 0; i < frames; ++i)
		{
			glBindTexture(GL_TEXTURE_2D, ids[i].id());
			glTexImage2D(GL_TEXTURE_2D, 0, internal_format(channels), width, height, 0, format(channels), GL_UNSIGNED_BYTE, data + i * area);
			if (generate_mipmaps)
				glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		stbi_image_free(delay_arr);
		stbi_image_free(data);
	}

	static std::unordered_map<gif_texture_ctor, std::weak_ptr<gif_texture>, texture_ctor_hash> gif_textures;

	std::shared_ptr<gif_texture> gif_texture::load(const std::string_view filepath, std::optional<GLenum> min_filter, std::optional<GLenum> mag_filter, bool generate_mipmaps)
	{
		gif_texture_ctor ctor{
			.filepath = filepath,
			.min_filter = min_filter ? *min_filter : GL_NEAREST,
			.mag_filter = mag_filter ? *mag_filter : GL_NEAREST,
			.generate_mipmaps = generate_mipmaps
		};

		auto it = gif_textures.find(ctor);
		if (it != gif_textures.end() && !it->second.expired())
			return it->second.lock();

		std::shared_ptr<gif_texture> sp(new gif_texture(filepath, ctor.min_filter, ctor.mag_filter, ctor.generate_mipmaps));
		gif_textures.emplace(ctor, sp);
		return sp;
	}

	void gif_texture::update()
	{
		const int frames = static_cast<int>(delays.size());
		timer += delta_seconds() * speed;
		while (timer > std::max(delays[index], 0.001f))
		{
			timer -= std::max(delays[index], 0.001f);
			index = (index + 1) % frames;
		}
		while (timer < 0.f)
		{
			timer += std::max(delays[index], 0.001f);
			index = (index - 1 + frames) % frames;
		}
	}

	GLuint gif_texture::get_id() const
	{
		return ids[index].id();
	}

	float gif_texture::get_width() const
	{
		return width;
	}

	float gif_texture::get_height() const
	{
		return height;
	}

	svg_texture::svg_texture(const std::string_view filepath, float scale, GLenum min_filter, GLenum mag_filter, bool generate_mipmaps)
	{
		NSVGimage* image = nsvgParseFromFile(filepath.data(), "px", 96.f);
		if (!image)
			throw error(error_code::load_texture, "Cannot parse svg from file: " + std::string(filepath));

		NSVGrasterizer* rasterizer = nsvgCreateRasterizer();
		if (!rasterizer)
		{
			nsvgDelete(image);
			throw error(error_code::load_texture, "Failed to create svg rasterizer");
		}

		width = std::max(static_cast<int>(scale * image->width), 1);
		height = std::max(static_cast<int>(scale * image->height), 1);
		const int channels = 4;
		const int stride = width * channels;
		unsigned char* data = new (std::nothrow) unsigned char[stride * height];
		if (!data)
		{
			nsvgDeleteRasterizer(rasterizer);
			nsvgDelete(image);
			throw error(error_code::load_texture, "Bad alloc");
		}

		nsvgRasterize(rasterizer, image, 0.0f, 0.0f, scale, data, width, height, stride);

		glBindTexture(GL_TEXTURE_2D, id.id());
		glTexImage2D(GL_TEXTURE_2D, 0, internal_format(channels), width, height, 0, format(channels), GL_UNSIGNED_BYTE, data);
		if (generate_mipmaps)
			glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
		glBindTexture(GL_TEXTURE_2D, 0);

		delete[] data;
		nsvgDeleteRasterizer(rasterizer);
		nsvgDelete(image);
	}

	static std::unordered_map<svg_texture_ctor, std::weak_ptr<svg_texture>, texture_ctor_hash> svg_textures;

	std::shared_ptr<svg_texture> svg_texture::load(const std::string_view filepath, float scale, std::optional<GLenum> min_filter, std::optional<GLenum> mag_filter, bool generate_mipmaps)
	{
		svg_texture_ctor ctor{
			.filepath = filepath,
			.min_filter = min_filter ? *min_filter : GL_LINEAR,
			.mag_filter = mag_filter ? *mag_filter : GL_LINEAR,
			.scale = scale,
			.generate_mipmaps = generate_mipmaps
		};

		auto it = svg_textures.find(ctor);
		if (it != svg_textures.end() && !it->second.expired())
			return it->second.lock();

		std::shared_ptr<svg_texture> sp(new svg_texture(filepath, ctor.scale, ctor.min_filter, ctor.mag_filter, ctor.generate_mipmaps));
		svg_textures.emplace(ctor, sp);
		return sp;
	}

	GLuint svg_texture::get_id() const
	{
		return id.id();
	}

	float svg_texture::get_width() const
	{
		return width * preview_scale;
	}

	float svg_texture::get_height() const
	{
		return height * preview_scale;
	}

	bool texture::empty() const
	{
		return std::get_if<std::monostate>(&v);
	}

	raster_texture* texture::get_raster()
	{
		auto ptr = std::get_if<std::shared_ptr<raster_texture>>(&v);
		return ptr ? ptr->get() : nullptr;
	}

	gif_texture* texture::get_gif()
	{
		auto ptr = std::get_if<std::shared_ptr<gif_texture>>(&v);
		return ptr ? ptr->get() : nullptr;
	}

	svg_texture* texture::get_svg()
	{
		auto ptr = std::get_if<std::shared_ptr<svg_texture>>(&v);
		return ptr ? ptr->get() : nullptr;
	}

	GLuint texture::id() const
	{
		return std::visit([](const auto& t) -> GLuint {
			if constexpr (std::is_same_v<std::decay_t<decltype(t)>, std::monostate>)
				return 0;
			else
				return t->get_id();
			}, v);
	}

	float texture::width() const
	{
		return std::visit([](const auto& t) {
			if constexpr (std::is_same_v<std::decay_t<decltype(t)>, std::monostate>)
				return 0.f;
			else
				return t->get_width();
			}, v);
	}

	float texture::height() const
	{
		return std::visit([](const auto& t) {
			if constexpr (std::is_same_v<std::decay_t<decltype(t)>, std::monostate>)
				return 0.f;
			else
				return t->get_height();
			}, v);
	}

	texture texture::load_generic(const std::string_view filepath, std::optional<GLenum> min_filter, std::optional<GLenum> mag_filter, float scale, bool generate_mipmaps)
	{
		texture t;
		if (filepath.ends_with(".svg"))
			t.v = svg_texture::load(filepath, scale, min_filter, mag_filter, generate_mipmaps);
		else if (filepath.ends_with(".gif"))
			t.v = gif_texture::load(filepath, min_filter, mag_filter, generate_mipmaps);
		else
			t.v = raster_texture::load(filepath, min_filter, mag_filter, generate_mipmaps);
		return t;
	}

	void texture::update_textures()
	{
		static const auto prune = [](auto& textures) {
			for (auto it = textures.begin(); it != textures.end();)
			{
				if (it->second.expired())
					it = textures.erase(it);
				else
					++it;
			}
		};

		prune(raster_textures);
		prune(gif_textures);
		prune(svg_textures);
	}
}
