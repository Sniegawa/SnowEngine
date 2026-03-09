#pragma once

#include <cstdint>
#include <string>

#include "SnowEngineAPI.h"
#include <glm/glm.hpp>

namespace Snow
{

	enum class TextureFormat
	{
		RGBA = 0,
		RGB
	};

	enum class TextureFilter
	{
		Nearest = 0,
		Linear
	};

	enum class TextureWrap
	{
		Clamp = 0,
		Repeat
	};

	typedef struct TextureParameters
	{
		TextureFormat Format;
		TextureWrap Wrap;
		TextureFilter MinFilter;
		TextureFilter MagFilter;

		glm::vec3 Tint = glm::vec3(1.0f);
		float Opacity = 1.0f;

		TextureParameters(
			TextureFormat format = TextureFormat::RGBA,
			TextureFilter minFilter = TextureFilter::Nearest,
			TextureFilter magFilter = TextureFilter::Nearest,
			TextureWrap wrap = TextureWrap::Repeat,
			glm::vec3 tint = glm::vec3(1.0f),
			float opacity = 1.0f
		) : Format(format),MinFilter(minFilter),MagFilter(magFilter),Wrap(wrap),Tint(tint),Opacity(opacity) {}
	} TextureParameters;


	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		const glm::vec2 GetSize() const { return glm::vec2(GetWidth(), GetHeight()); }
		
		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual void SetData(const void* data, uint32_t size) = 0;

		virtual const uint32_t GetRendererID() const = 0;

		virtual void Reload(const TextureParameters& params) = 0;

		const inline TextureParameters& GetTextureParameters() const { return m_Params; }

		const inline glm::vec3& GetTextureTint() const { return m_Params.Tint; }
		void SetTextureTint(const glm::vec3& tint) { m_Params.Tint = tint; }

		const inline float GetTextureOpacity() const { return m_Params.Opacity; }
		void SetTextureOpacity(float opacity) { m_Params.Opacity = opacity; }

	protected:
		TextureParameters m_Params;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(const Path& path, TextureParameters params = TextureParameters(TextureFormat::RGBA));
		static Ref<Texture2D> Create(uint32_t width, uint32_t height, TextureParameters params = TextureParameters(TextureFormat::RGBA));
		virtual bool operator==(const Texture2D& other) const = 0;
	};
};