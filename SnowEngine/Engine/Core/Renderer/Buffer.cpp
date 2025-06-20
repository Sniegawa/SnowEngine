#include "Buffer.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Core/Logging/Log.h"

namespace Snow
{
	VertexBuffer* VertexBuffer::Create(float* vertices, size_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:		SNOW_CORE_ERROR("Renderer API is set to none");		return nullptr;
		case RendererAPI::OpenGL:	return new OpenGLVertexBuffer(vertices,size);
			
		default:	SNOW_CORE_ERROR("Can't get Renderer API");	break;
		}
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch(Renderer::GetAPI())
		{
		case RendererAPI::None:		SNOW_CORE_ERROR("Renderer API is set to none");		return nullptr;
		case RendererAPI::OpenGL:	return new OpenGLIndexBuffer(indices, count);

		default:	SNOW_CORE_ERROR("Can't get Renderer API");	break;
		}
	}
};