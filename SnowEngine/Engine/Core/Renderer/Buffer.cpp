
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Core/Logging/Log.h"

namespace Snow
{
	Ref<VertexBuffer> VertexBuffer::Create(size_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:		SNOW_CORE_ERROR("Renderer API is set to none");		return nullptr;
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLVertexBuffer>(size);

		default:	SNOW_CORE_ERROR("Can't get Renderer API");	return nullptr;
		}
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, size_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:		SNOW_CORE_ERROR("Renderer API is set to none");		return nullptr;
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLVertexBuffer>(vertices,size);
			
		default:	SNOW_CORE_ERROR("Can't get Renderer API");	return nullptr;
		}
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch(Renderer::GetAPI())
		{
		case RendererAPI::API::None:		SNOW_CORE_ERROR("Renderer API is set to none");		return nullptr;
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLIndexBuffer>(indices, count);

		default:	SNOW_CORE_ERROR("Can't get Renderer API");	return nullptr;
		}
	}
};
