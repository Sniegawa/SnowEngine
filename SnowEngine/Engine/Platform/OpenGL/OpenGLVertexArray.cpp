#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Snow
{
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case Snow::ShaderDataType::None:		break;

		case Snow::ShaderDataType::Float:		return GL_FLOAT;
		case Snow::ShaderDataType::FLoat2:		return GL_FLOAT;
		case Snow::ShaderDataType::Float3:		return GL_FLOAT;
		case Snow::ShaderDataType::Float4:		return GL_FLOAT;

		case Snow::ShaderDataType::Mat3:		return GL_FLOAT;
		case Snow::ShaderDataType::Mat4:		return GL_FLOAT;

		case Snow::ShaderDataType::Int:			return GL_INT;
		case Snow::ShaderDataType::Int2:		return GL_INT;
		case Snow::ShaderDataType::Int3:		return GL_INT;
		case Snow::ShaderDataType::Int4:		return GL_INT;

		case Snow::ShaderDataType::Bool:		return GL_BOOL;
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

	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		SNOW_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, 
				element.GetComponentCount(), 
				ShaderDataTypeToOpenGLBaseType(element.Type), 
				element.Normalized ? GL_TRUE : GL_FALSE, 
				layout.GetStride(), 
				(const void*)element.Offset);
			index++;
		}
		glBindVertexArray(0);

		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();
		glBindVertexArray(0);
		m_IndexBuffer = indexBuffer;
	}

};