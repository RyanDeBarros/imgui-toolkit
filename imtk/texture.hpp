#pragma once

#include "external/gl.hpp"

#include <memory>
#include <optional>
#include <string_view>
#include <variant>
#include <vector>

namespace imtk
{
	class texture_id
	{
		GLuint _id = 0;

	public:
		texture_id();
		texture_id(const texture_id&) = delete;
		texture_id(texture_id&&) noexcept;
		~texture_id();
		texture_id& operator=(texture_id&&) noexcept;

		GLuint id() const;
	};

	struct raster_texture
	{
		texture_id id;
		int width = 0, height = 0;

	private:
		raster_texture(const std::string_view filepath, GLenum min_filter, GLenum mag_filter, bool generate_mipmaps);

	public:
		static std::shared_ptr<raster_texture> load(const std::string_view filepath, std::optional<GLenum> min_filter = std::nullopt,
			std::optional<GLenum> mag_filter = std::nullopt, bool generate_mipmaps = false);

		GLuint get_id() const;
		float get_width() const;
		float get_height() const;
	};

	struct gif_texture
	{
		std::vector<texture_id> ids;
		std::vector<float> delays;
		int width = 0, height = 0;
		int index = 0;
		float timer = 0.f;
		float speed = 1.f;

	private:
		gif_texture(const std::string_view filepath, GLenum min_filter, GLenum mag_filter, bool generate_mipmaps);

	public:
		static std::shared_ptr<gif_texture> load(const std::string_view filepath, std::optional<GLenum> min_filter = std::nullopt,
			std::optional<GLenum> mag_filter = std::nullopt, bool generate_mipmaps = false);

		void update();

		GLuint get_id() const;
		float get_width() const;
		float get_height() const;
	};

	struct svg_texture
	{
		texture_id id;
		int width = 0, height = 0;
		float preview_scale = 1.f;

	private:
		svg_texture(const std::string_view filepath, float scale, GLenum min_filter, GLenum mag_filter, bool generate_mipmaps);

	public:
		static std::shared_ptr<svg_texture> load(const std::string_view filepath, float scale = 1.f, std::optional<GLenum> min_filter = std::nullopt,
			std::optional<GLenum> mag_filter = std::nullopt, bool generate_mipmaps = false);

		GLuint get_id() const;
		float get_width() const;
		float get_height() const;
	};

	struct texture
	{
		using variant = std::variant<std::monostate, std::shared_ptr<raster_texture>, std::shared_ptr<gif_texture>, std::shared_ptr<svg_texture>>;
		variant v;

		bool empty() const;
		raster_texture* get_raster();
		gif_texture* get_gif();
		svg_texture* get_svg();
		GLuint id() const;
		float width() const;
		float height() const;

		static texture load_generic(const std::string_view filepath, std::optional<GLenum> min_filter = std::nullopt,
			std::optional<GLenum> mag_filter = std::nullopt, float scale = 1.f, bool generate_mipmaps = false);

		static void update_textures();
	};
}
