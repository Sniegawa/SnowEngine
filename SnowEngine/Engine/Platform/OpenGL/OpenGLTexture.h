#pragma once

#include "Core/Renderer/Texture.h"
#include <glad/glad.h>
namespace Snow
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height,const TextureParameters& params);
		OpenGLTexture2D(const std::string& path, const TextureParameters& params);
		virtual ~OpenGLTexture2D();
		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }

		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot = 0) const override;

		virtual bool operator==(const Texture2D& other) const override 
		{ 
			return m_RendererID == ((OpenGLTexture2D&)other).m_RendererID;
		}
	private:
		uint32_t m_Width;
		uint32_t m_Height;
		std::string m_Path;
		uint32_t m_RendererID;
		GLenum m_internalFormat, m_dataFormat;
	};
};