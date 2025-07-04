#pragma once

#include <cstdint>

#include "../Logging/Log.h"

namespace Snow
{
	enum class ShaderDataType
	{
		None = 0,
		Float,
		FLoat2,
		Float3,
		Float4,
		Mat3,
		Mat4,
		Int,
		Int2,
		Int3,
		Int4,
		Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
			case Snow::ShaderDataType::None:		break;

			case Snow::ShaderDataType::Float:		return 4;
			case Snow::ShaderDataType::FLoat2:		return 4 * 2;
			case Snow::ShaderDataType::Float3:		return 4 * 3;
			case Snow::ShaderDataType::Float4:		return 4 * 4;

			case Snow::ShaderDataType::Mat3:		return 4 * 3 * 3;
			case Snow::ShaderDataType::Mat4:		return 4 * 4 * 4;

			case Snow::ShaderDataType::Int:			return 4;
			case Snow::ShaderDataType::Int2:		return 4 * 2;
			case Snow::ShaderDataType::Int3:		return 4 * 3;
			case Snow::ShaderDataType::Int4:		return 4 * 4;

			case Snow::ShaderDataType::Bool:		return 1;
		}

		SNOW_ASSERT(false,"Unknown ShaderDataType!");
		return 0;
	}


	struct BufferElement
	{
		std::string Name;
		ShaderDataType Type;
		uint32_t Offset;
		uint32_t Size;
		bool Normalized;

		BufferElement(){}

		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			: Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0) , Normalized(normalized)
		{

		}

		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
				case Snow::ShaderDataType::None:		break;

				case Snow::ShaderDataType::Float:		return 1;
				case Snow::ShaderDataType::FLoat2:		return 2;
				case Snow::ShaderDataType::Float3:		return 3;
				case Snow::ShaderDataType::Float4:		return 4;

				case Snow::ShaderDataType::Mat3:		return 3 * 3;
				case Snow::ShaderDataType::Mat4:		return 4 * 4;

				case Snow::ShaderDataType::Int:			return 1;
				case Snow::ShaderDataType::Int2:		return 2;
				case Snow::ShaderDataType::Int3:		return 3;
				case Snow::ShaderDataType::Int4:		return 4;

				case Snow::ShaderDataType::Bool:		return 1;
			}
			return 0;
		}
	};

	class BufferLayout
	{
	public:

		BufferLayout(const std::initializer_list<BufferElement>& elements)
			: m_Elements(elements)
		{
			CalculateOffsetsAndStride();
		}

		BufferLayout(){}

		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }

		inline const uint32_t GetStride() const { return m_Stride; }

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
	private:
		void CalculateOffsetsAndStride()
		{
			uint32_t offset = 0;
			m_Stride = 0;
			for (auto& element : m_Elements)
			{
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}
	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;
	};


	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		
		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;

		virtual void SetData(const void* data, uint32_t size = 0) = 0;

		static Ref<VertexBuffer> Create(size_t size);
		static Ref<VertexBuffer> Create(float* vertices, size_t size);
	};

	//TODO : add support for 16-bit index buffers to save memory
	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() const = 0;

		static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t count);
	};


};