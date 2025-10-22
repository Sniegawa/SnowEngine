#include "Framebuffer.h"

#include "Renderer.h"
#include "Core/Logging/Log.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Snow
{

	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& specs)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:		SNOW_CORE_ERROR("Renderer API is set to none");		return nullptr;
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLFramebuffer>(OpenGLFramebuffer(specs));

		default:	SNOW_CORE_ERROR("Can't get Renderer API");	return nullptr;
		}
	}
};
