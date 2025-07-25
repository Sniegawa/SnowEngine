#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Snow
{
	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height, TextureParameters params)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:		SNOW_CORE_ERROR("Renderer API is set to none");		return nullptr;
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLTexture2D>(width,height,params);

		default:	SNOW_CORE_ERROR("Can't get Renderer API");	break;
		}
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path, TextureParameters params)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:		SNOW_CORE_ERROR("Renderer API is set to none");		return nullptr;
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLTexture2D>(path, params);

		default:	SNOW_CORE_ERROR("Can't get Renderer API");	break;
		}
		return nullptr;
	}
};