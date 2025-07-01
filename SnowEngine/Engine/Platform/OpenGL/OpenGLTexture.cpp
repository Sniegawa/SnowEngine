#include "OpenGLTexture.h"

#include <glad/glad.h>
#include "stb_image.h"

#include "SnowEngineAPI.h"

namespace Snow
{

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path,const TextureParameters& params)
		: m_Path(path)
	{
		int width, height,channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels,0);

		SNOW_CORE_ASSERT(data, "Failed to load image from path : " + path);
		
		m_Width = width;
		m_Height = height;


		GLenum internalFormat = GL_RGBA8, dataFormat = GL_RGBA;
		switch (channels)
		{
		case 2:
			dataFormat = GL_RG; internalFormat = GL_RG8; break;
		case 3:
			dataFormat = GL_RGB; internalFormat = GL_RGB8; break;
		case 4:
			dataFormat = GL_RGBA; internalFormat = GL_RGBA8; break;
		default:
			SNOW_CORE_WARN("Cannot deduce desired texture format, channels given : {0}", channels); 
			dataFormat = GL_RGBA; break;
		}

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

	
		//Realy unreadable piece of crap
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, params.MinFilter == SNOW_TEXTURE_NEAREST ? GL_NEAREST : GL_LINEAR); 
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, params.MagFilter == SNOW_TEXTURE_NEAREST ? GL_NEAREST : GL_LINEAR);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, params.Wrap == SNOW_TEXTURE_REPEAT ? GL_REPEAT : GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, params.Wrap == SNOW_TEXTURE_REPEAT ? GL_REPEAT : GL_CLAMP_TO_EDGE);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1,&m_RendererID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}
}