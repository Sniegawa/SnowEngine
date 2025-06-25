#include "Texture.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"
namespace Snow
{
	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:		SNOW_CORE_ERROR("Renderer API is set to none");		return nullptr;
		case RendererAPI::API::OpenGL:	return std::make_shared<OpenGLTexture2D>(path);

		default:	SNOW_CORE_ERROR("Can't get Renderer API");	break;
		}
	}
};