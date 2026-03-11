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
        SNOW_CORE_WARN("Could not resolve GL format from texture parameters");
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
        SNOW_CORE_WARN("Could not resolve GL internal format from texture parameters");
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
        SNOW_CORE_WARN("Could not resolve Texture filter");
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
        SNOW_CORE_WARN("Could not resolve texture wrap");
				return GL_REPEAT;
				break;
			}
		}
	};

	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height, const TextureParameters& params)
		: m_Width(width),m_Height(height)
	{
		m_Params = params;
		Initialize();
	}


	OpenGLTexture2D::OpenGLTexture2D(const Path& path, const TextureParameters& params) // Deprecated
	{
		int width, height,channels;
		std::string PathString = path.string();
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(PathString.c_str(), &width, &height, &channels, 0);

		SNOW_CORE_ASSERT(data, "Failed to load image from path : " + PathString);
		
		m_Width = width;
		m_Height = height;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1,TextureUtils::GetGLInternalFormatFromParams(params), m_Width, m_Height);


		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, TextureUtils::GetGLTextureFilter(params.MinFilter));
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, TextureUtils::GetGLTextureFilter(params.MagFilter));

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, TextureUtils::GetGLTextureWrap(params.Wrap));
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, TextureUtils::GetGLTextureWrap(params.Wrap));



		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, TextureUtils::GetGLFormatFromParams(m_Params), GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	
	}

	void OpenGLTexture2D::SetData(const void* data, uint32_t size)
	{
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, TextureUtils::GetGLFormatFromParams(m_Params), GL_UNSIGNED_BYTE, data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1,&m_RendererID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}

	void OpenGLTexture2D::Reload(const TextureParameters& params)
	{
		m_Params = params;

		glDeleteTextures(1, &m_RendererID);
		Initialize();
	}

	void OpenGLTexture2D::Initialize()
	{
		auto internalFormat = TextureUtils::GetGLInternalFormatFromParams(m_Params);
		auto dataFormat = TextureUtils::GetGLFormatFromParams(m_Params);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, TextureUtils::GetGLTextureFilter(m_Params.MinFilter));
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, TextureUtils::GetGLTextureFilter(m_Params.MagFilter));

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, TextureUtils::GetGLTextureWrap(m_Params.Wrap));
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, TextureUtils::GetGLTextureWrap(m_Params.Wrap));
	}
}
