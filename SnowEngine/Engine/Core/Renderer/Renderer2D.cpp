#include "Renderer2D.h"

#include "VertexArray.h"
#include "RenderCommand.h"


#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Snow
{
	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> FlatColorShader;
		Ref<Shader> TextureShader;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		s_Data = new Renderer2DStorage();
		s_Data->QuadVertexArray = VertexArray::Create();

		float verticesSquare[4 * 5] = {
			-0.5f,-0.5f,0.0f, 0.0f, 0.0f,
			0.5f,-0.5f,0.0f, 1.0f, 0.0f,
			0.5f,0.5f,0.0f, 1.0f, 1.0f,
			-0.5f,0.5f,0.0f, 0.0f, 1.0f
		};

		Ref<VertexBuffer> vb(VertexBuffer::Create(verticesSquare, sizeof(verticesSquare)));

		BufferLayout Squarelayout =
		{
			{ShaderDataType::Float3,"a_Position"},
			{ShaderDataType::FLoat2,"a_TexCoord"}
		};

		vb->SetLayout(Squarelayout);

		s_Data->QuadVertexArray->AddVertexBuffer(vb);

		uint32_t indicesSquare[6] = { 0,1,2,2,3,0 };
		Ref<IndexBuffer> ib(IndexBuffer::Create(indicesSquare, sizeof(indicesSquare) / sizeof(uint32_t)));

		s_Data->QuadVertexArray->SetIndexBuffer(ib);

		s_Data->FlatColorShader = Shader::Create("PlainColor", "Assets/Shaders/PlainColor.vert", "Assets/Shaders/PlainColor.frag", true);
		s_Data->TextureShader = Shader::Create("Texture", "Assets/Shaders/Texture.vert", "Assets/Shaders/Texture.frag", true);

	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(const Camera& camera)
	{
		s_Data->FlatColorShader->Bind();
		s_Data->FlatColorShader->UploadUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->UploadUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{
		
	}

	//Primitives
	void Renderer2D::DrawQuad(glm::vec2 pos, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawQuad({ pos.x,pos.y,0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawQuad(glm::vec3 pos, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos);
		transform = glm::rotate(transform, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		transform = glm::scale(transform, glm::vec3(size.x, size.y, 1.0f));
		s_Data->FlatColorShader->Bind();
		s_Data->FlatColorShader->UploadUniformMat4("u_ModelMatrix", transform);
		s_Data->FlatColorShader->UploadUniformFloat4("u_Color", color);

		s_Data->QuadVertexArray->Bind();

		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(glm::vec2 pos, const glm::vec2& size, float rotation, Ref<Texture>& texture)
	{
		DrawQuad({ pos.x,pos.y,0.0f }, size, rotation, texture);
	}

	void Renderer2D::DrawQuad(glm::vec3 pos, const glm::vec2& size, float rotation, Ref<Texture>& texture)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y, 0.0f));
		transform = glm::rotate(transform, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		transform = glm::scale(transform, glm::vec3(size.x, size.y, 1.0f));

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->UploadUniformMat4("u_ModelMatrix", transform);
		s_Data->TextureShader->UploadUniformInt("u_Texture", 0); // Should be proper texture unit
		texture->Bind();

		s_Data->QuadVertexArray->Bind();

		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

};