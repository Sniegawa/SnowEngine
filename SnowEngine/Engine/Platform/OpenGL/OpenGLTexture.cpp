#include "OpenGLTexture.h"

#include <glad/glad.h>
#include "stb_image.h"

#include "SnowEngineAPI.h"

namespace Snow
{

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		: m_Path(path)
	{
		int width, height,channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels,0);

		SNOW_CORE_ASSERT(data, "Failed to load image from path : " + path);
		
		m_Width = width;
		m_Height = height;


		GLenum internalFormat, dataFormat;
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

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);



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