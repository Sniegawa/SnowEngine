#include "Shader.h"

#include "Renderer.h"
#include "Core/Logging/Log.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Snow
{
	Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:		SNOW_CORE_ERROR("Renderer API is set to none");		return nullptr;
		case RendererAPI::API::OpenGL:	return new OpenGLShader(vertexSrc,fragmentSrc);

		default:	SNOW_CORE_ERROR("Can't get Renderer API");	break;
		}
		SNOW_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}
};