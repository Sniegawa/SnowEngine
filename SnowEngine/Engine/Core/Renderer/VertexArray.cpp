#include "VertexArray.h"

#include "Renderer.h"
#include "Core/Logging/Log.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Snow
{
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:		SNOW_CORE_ERROR("Renderer API is set to none");		return nullptr;
		case RendererAPI::OpenGL:	return new OpenGLVertexArray();

		default:	SNOW_CORE_ERROR("Can't get Renderer API");	break;
		}
	}
};