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
		TextureFilter MinFilter;
		TextureFilter MagFilter;
		TextureFormat Format;
		TextureWrap Wrap;

		TextureParameters(
			TextureFormat format, 
			TextureFilter minFilter = TextureFilter::Nearest, 
			TextureFilter magFilter = TextureFilter::Nearest, 
			TextureWrap wrap = TextureWrap::Repeat
		) : Format(format),MinFilter(minFilter),MagFilter(magFilter),Wrap(wrap) {}
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

		//Might need to move it somewhere else in the future
		const glm::vec3& GetTextureTint() const { return m_TextureTint; }
		void SetTextureTint(glm::vec3& tint) { m_TextureTint = tint; }

		const float& GetOpacity() const { return m_Opacity; }
		void SetTextureOpacity(float& opacity) { m_Opacity = opacity; }

	private:
		glm::vec3 m_TextureTint = glm::vec3(1.0f);
		float m_Opacity = 1.0f;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(const std::string& path, TextureParameters params = TextureParameters(TextureFormat::RGBA));
		static Ref<Texture2D> Create(uint32_t width, uint32_t height, TextureParameters params = TextureParameters(TextureFormat::RGBA));
		virtual bool operator==(const Texture2D& other) const = 0;
	};
};