#include "Renderer2D.h"

#include "VertexArray.h"
#include "RenderCommand.h"


#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Snow
{

	struct QuadVertex
	{
		glm::vec3 Position = glm::vec3(0.0);
		glm::vec4 Color = glm::vec4(0.0);
		glm::vec2 TexCoord = glm::vec2(0.0);
		int TexIndex = 0;

		// Editor-only
		int EntityID = -1;
	};

	struct Renderer2DData
	{
		const static uint32_t MaxQuads = 10000;
		const static uint32_t MaxVertices = MaxQuads * 4;
		const static uint32_t MaxIndices = MaxQuads * 6;
		const static uint32_t MaxTextureSlots = 32;

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> TextureShader;
		Ref<Texture2D> DefaultTexture;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 - DefaultTexture

		glm::vec4 QuadVertexPositions[4];

		Renderer2D::Statistics stats;
	};

	static Renderer2DData s_Data;

	void Renderer2D::Init()
	{

		s_Data.QuadVertexArray = VertexArray::Create();

		s_Data.DefaultTexture = Texture2D::Create(1,1);
		uint32_t standardTexData = 0xFFFFFFFF;
		s_Data.DefaultTexture->SetData(&standardTexData, sizeof(uint32_t));

		s_Data.QuadVertexBuffer = VertexBuffer::Create(Renderer2DData::MaxVertices * sizeof(QuadVertex));

		s_Data.QuadVertexBuffer->SetLayout(
		{
			{ShaderDataType::Float3,"a_Position"},
			{ShaderDataType::Float4,"a_Color"},
			{ShaderDataType::Float2,"a_TexCoord"},
			{ShaderDataType::Int,"a_TextureIndex"},
			{ShaderDataType::Int,"a_EntityID"}
		});

		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		s_Data.QuadVertexBufferBase = new QuadVertex[Renderer2DData::MaxVertices];

		std::vector<uint32_t> quadIndices(Renderer2DData::MaxIndices);

		uint32_t offset = 0;
		for (uint32_t i = 0; i < Renderer2DData::MaxIndices; i+=6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;
			offset += 4;
		}

		Ref<IndexBuffer> ib(IndexBuffer::Create(quadIndices.data(), Renderer2DData::MaxIndices));
		s_Data.QuadVertexArray->SetIndexBuffer(ib);

		s_Data.TextureShader = Shader::Create("Texture", "Assets/Shaders/Texture.vert", "Assets/Shaders/Texture.frag", true); // Todo : don't staticly link path

		int32_t samplers[s_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
		{
			samplers[i] = i;
		}
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->UploadUniformIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);
		
		s_Data.TextureSlots[0] = s_Data.DefaultTexture;

		s_Data.QuadVertexPositions[0] = {-0.5f,-0.5f,0.0f,1.0f};
		s_Data.QuadVertexPositions[1] = {0.5f,-0.5f,0.0f,1.0f};
		s_Data.QuadVertexPositions[2] = {0.5f,0.5f,0.0f,1.0f};
		s_Data.QuadVertexPositions[3] = {-0.5f,0.5f,0.0f,1.0f};
		
	}

	void Renderer2D::Shutdown()
	{
		s_Data.QuadVertexBuffer.reset();
		s_Data.QuadVertexArray.reset();
		s_Data.TextureShader.reset();
		s_Data.DefaultTexture.reset();
		delete s_Data.QuadVertexBufferBase;
	}

	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		glm::mat4 viewProje = camera.GetProjectionMatrix() * glm::inverse(transform);
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->UploadUniformMat4("u_ViewProjection", viewProje);

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
	}

	void Renderer2D::BeginScene(const EditorCamera& camera)
	{
		glm::mat4 viewProj = camera.GetViewProjection();
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->UploadUniformMat4("u_ViewProjection", viewProj);

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
	}

	void Renderer2D::Flush()
	{
		//Future support for bindless texturing
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
			s_Data.TextureSlots[i]->Bind(i);

		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);

		if (dataSize == 0)
			return;

		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);


		RenderCommand::DrawIndexed(s_Data.QuadVertexArray,s_Data.QuadIndexCount);

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
		s_Data.TextureSlotIndex = 1;

		s_Data.stats.DrawCalls++;
	}

	void Renderer2D::EndScene()
	{
		Flush();
	}

	//Primitives
	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad(glm::vec3(pos.x, pos.y, 0.0f), size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)
			* glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));

		DrawQuad(transform, color);
	}

	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size, Ref<Texture2D>& texture)
	{
		DrawQuad(glm::vec3(pos.x, pos.y, 0.0f), size, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, Ref<Texture2D>& texture)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)
			* glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));

		DrawQuad(transform, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size, Ref<Subtexture2D>& subTexture)
	{
		DrawQuad(glm::vec3(pos.x, pos.y, 0.0f), size, subTexture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, Ref<Subtexture2D>& subTexture)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)
			* glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));

		DrawQuad(transform, subTexture);
	}


	void Renderer2D::DrawRotatedQuad(const glm::vec2& pos, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotatedQuad(glm::vec3(pos.x, pos.y, 0.0f), size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& pos, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f))
			* glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));

		DrawQuad(transform, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& pos, const glm::vec2& size, float rotation, Ref<Texture2D>& texture)
	{
		DrawRotatedQuad(glm::vec3(pos.x, pos.y, 0.0f), size, rotation, texture);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& pos, const glm::vec2& size, float rotation, Ref<Texture2D>& texture)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f))
			* glm::scale(glm::mat4(1.0f), glm::vec3(size,1.0f));

		DrawQuad(transform, texture);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& pos, const glm::vec2& size, float rotation, Ref<Subtexture2D>& subTexture)
	{
		glm::vec3 Pos = glm::vec3(pos.x,pos.y,0.0f);
		DrawRotatedQuad(Pos, size, rotation, subTexture);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& pos, const glm::vec2& size, float rotation, Ref<Subtexture2D>& subTexture)
	{

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f))
			* glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));
		
		DrawQuad(transform, subTexture);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		if (s_Data.QuadIndexCount + 6 > s_Data.MaxIndices)
			Flush();

		const int texIndex = 0;//Default texture - for colors
		const glm::vec2 VertexTexCoords[4] = { {0.0f,0.0f},{1.0f,0.0f},{1.0f,1.0f},{0.0f,1.0f} };

		for (uint32_t i = 0; i < 4; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = VertexTexCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
			s_Data.QuadVertexBufferPtr++;
		}
		s_Data.QuadIndexCount += 6;

		s_Data.stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, Ref<Texture2D>& texture)
	{
		glm::vec4 color = glm::vec4(texture->GetTextureTint(), texture->GetOpacity());

		if (s_Data.QuadIndexCount + 6 > s_Data.MaxIndices)
			Flush();

		int textureIndex = 0;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i].get() == *texture.get()) // Comparison between textures
			{
				textureIndex = i;
				break;
			}
		}

		if (textureIndex == 0)
		{
			if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
				Flush();
			textureIndex = s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}
		const glm::vec2 VertexTexCoords[4] = { {0.0f,0.0f},{1.0f,0.0f},{1.0f,1.0f},{0.0f,1.0f} };

		for (uint32_t i = 0; i < 4; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = VertexTexCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr++;
		}
		s_Data.QuadIndexCount += 6;

		s_Data.stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, Ref<Subtexture2D>& subTexture)
	{
		Ref<Texture2D> texture = subTexture->GetTexture();
		glm::vec4 color = glm::vec4(texture->GetTextureTint(), texture->GetOpacity());

		if (s_Data.QuadIndexCount + 6 > s_Data.MaxIndices)
			Flush();

		int textureIndex = 0;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i].get() == *texture.get()) // Comparison between textures
			{
				textureIndex = i;
				break;
			}
		}

		if (textureIndex == 0)
		{
			if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
				Flush();
			textureIndex = s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		for (uint32_t i = 0; i < 4; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = subTexture->GetTexCoords()[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr++;
		}
		s_Data.QuadIndexCount += 6;

		s_Data.stats.QuadCount++;
	}

	void Renderer2D::DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityID)
	{
		if (s_Data.QuadIndexCount + 6 > s_Data.MaxIndices)
			Flush();

		const int texIndex = 0;//Default texture - for colors
		const glm::vec2 VertexTexCoords[4] = { {0.0f,0.0f},{1.0f,0.0f},{1.0f,1.0f},{0.0f,1.0f} };

		for (uint32_t i = 0; i < 4; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = src.Color;
			s_Data.QuadVertexBufferPtr->TexCoord = VertexTexCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
			s_Data.QuadVertexBufferPtr->EntityID = entityID;
			s_Data.QuadVertexBufferPtr++;
		}
		s_Data.QuadIndexCount += 6;

		s_Data.stats.QuadCount++;
	}

	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return s_Data.stats;
	}
	void Renderer2D::ResetStats()
	{
		memset(&s_Data.stats, 0, sizeof(Statistics));
	}

};