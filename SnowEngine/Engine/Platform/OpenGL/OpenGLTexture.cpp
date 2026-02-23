#include "OpenGLTexture.h"
#include "stb_image.h"

#include "SnowEngineAPI.h"

namespace Snow
{
	namespace TextureUtils
	{
		GLenum GetGLFormatFromParams(const TextureParameters& params)
		{
			switch (params.Format)
			{
			case TextureFormat::RGB:
				return GL_RGB;
				break;
			case TextureFormat::RGBA:
				return GL_RGBA;
				break;
			default:
				return GL_RGBA;
				break;
			}
		}
		GLenum GetGLInternalFormatFromParams(const TextureParameters& params)
		{
			switch (params.Format)
			{
			case TextureFormat::RGB:
				return GL_RGB8;
				break;
			case TextureFormat::RGBA:
				return GL_RGBA8;
				break;
			default:
				return GL_RGBA8;
				break;
			}
		}

		GLenum GetGLTextureFilter(const TextureFilter& filter)
		{
			switch (filter)
			{
			case TextureFilter::Nearest:
				return GL_NEAREST;
				break;
			case TextureFilter::Linear:
				return GL_LINEAR;
				break;
			default:
				return GL_NEAREST;
				break;
			}
		}

		GLenum GetGLTextureWrap(const TextureWrap& wrap)
		{
			switch (wrap)
			{
			case TextureWrap::Clamp:
				return GL_CLAMP_TO_EDGE;
				break;
			case TextureWrap::Repeat:
				return GL_REPEAT;
				break;
			default:
				return GL_REPEAT;
				break;
			}
		}
	};

	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height, const TextureParameters& params)
		: m_Width(width),m_Height(height)
	{
		m_internalFormat = TextureUtils::GetGLInternalFormatFromParams(params);
		m_dataFormat = TextureUtils::GetGLFormatFromParams(params);
		
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_internalFormat, m_Width, m_Height);

		//Really unreadable piece of crapa
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, TextureUtils::GetGLTextureFilter(params.MinFilter));
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, TextureUtils::GetGLTextureFilter(params.MagFilter));

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, TextureUtils::GetGLTextureWrap(params.Wrap));
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, TextureUtils::GetGLTextureWrap(params.Wrap));
	}


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

		m_internalFormat = internalFormat;
		m_dataFormat = dataFormat;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_internalFormat, m_Width, m_Height);

	
		//Realy unreadable piece of crap
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, TextureUtils::GetGLTextureFilter(params.MinFilter));
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, TextureUtils::GetGLTextureFilter(params.MagFilter));

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, TextureUtils::GetGLTextureWrap(params.Wrap));
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, TextureUtils::GetGLTextureWrap(params.Wrap));

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	
	}

	void OpenGLTexture2D::SetData(const void* data, uint32_t size)
	{
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_dataFormat, GL_UNSIGNED_BYTE, data);
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