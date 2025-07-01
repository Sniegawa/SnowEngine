#pragma once

#include <cstdint>
#include <string>

#include "SnowEngineAPI.h"
#include <glm/glm.hpp>

namespace Snow
{

#define SNOW_TEXTURE_NEAREST 0
#define SNOW_TEXTURE_LINEAR 1
#define SNOW_TEXTURE_CLAMP 0
#define SNOW_TEXTURE_REPEAT 1

	typedef struct TextureParameters
	{
		int MagFilter = 0;
		int MinFilter = 0;
		int Wrap = 0;
		TextureParameters(int minFilter = SNOW_TEXTURE_NEAREST,int magFilter = SNOW_TEXTURE_NEAREST,int wrap = SNOW_TEXTURE_REPEAT) : MagFilter(magFilter),MinFilter(minFilter),Wrap(wrap){}
	} TextureParameters;


	class Texture
	{
	public:
		virtual ~Texture() = default;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;
		
		//Might need to move it somewhere else in the future
		glm::vec3& GetTextureTint() { return m_TextureTint; }
		void SetTextureTint(glm::vec3& tint) { m_TextureTint = tint; }
	private:
		glm::vec3 m_TextureTint = glm::vec3(1.0f);
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(const std::string& path, TextureParameters params = TextureParameters());
	};
};