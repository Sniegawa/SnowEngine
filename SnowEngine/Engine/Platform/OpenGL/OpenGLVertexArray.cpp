#include "OpenGLVertexArray.h"

#include <cstdint>
#include <glad/glad.h>

namespace Snow
{
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case Snow::ShaderDataType::None:		break;

		case Snow::ShaderDataType::Float:		return GL_FLOAT;
		case Snow::ShaderDataType::Float2:		return GL_FLOAT;
		case Snow::ShaderDataType::Float3:		return GL_FLOAT;
		case Snow::ShaderDataType::Float4:		return GL_FLOAT;

		case Snow::ShaderDataType::Mat3:		return GL_FLOAT;
		case Snow::ShaderDataType::Mat4:		return GL_FLOAT;

		case Snow::ShaderDataType::Int:			return GL_INT;
		case Snow::ShaderDataType::Int2:		return GL_INT;
		case Snow::ShaderDataType::Int3:		return GL_INT;
		case Snow::ShaderDataType::Int4:		return GL_INT;

		case Snow::ShaderDataType::Bool:		return GL_INT;
		}

		SNOW_CORE_ASSERT(false, "Unknown shaderdatatype");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_RendererID);

	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		SNOW_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index + m_VertexBufferIndexOffset);
			switch (ShaderDataTypeToOpenGLBaseType(element.Type))
			{
			case GL_FLOAT:
				glVertexAttribPointer(index + m_VertexBufferIndexOffset,
					element.GetComponentCount(),
					GL_FLOAT,
					element.Normalized ? GL_TRUE : GL_FALSE,
					layout.GetStride(),
          reinterpret_cast<const void *>(static_cast<uintptr_t>(element.Offset)));
				break;
			case GL_INT:
				glVertexAttribIPointer(index + m_VertexBufferIndexOffset,
					element.GetComponentCount(),
					ShaderDataTypeToOpenGLBaseType(element.Type),
					layout.GetStride(),
					reinterpret_cast<const void *>(static_cast<uintptr_t>(element.Offset)));
				
				break;
			default:
				SNOW_CORE_ASSERT(0, "Unsupported shader data type given");
			}
			index++;
		}
		glBindVertexArray(0);

		m_VertexBuffers.push_back(vertexBuffer);
		m_VertexBufferIndexOffset += layout.GetElements().size();
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();
		glBindVertexArray(0);
		m_IndexBuffer = indexBuffer;
	}

};
