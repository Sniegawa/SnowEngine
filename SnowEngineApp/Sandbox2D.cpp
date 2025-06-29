#include "Sandbox2D.h"

Sandbox2D::Sandbox2D()
	: Layer("Application Example Layer"),
		m_CameraController(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, (1280.0f / 720.0f), true)
{
	m_ShaderLib.Load("PlainColor", "Assets/Shaders/PlainColor.vert", "Assets/Shaders/PlainColor.frag", true);
	m_ShaderLib.Load("Texture", "Assets/Shaders/Texture.vert", "Assets/Shaders/Texture.frag", true);
}

Sandbox2D::~Sandbox2D()
{

}

void Sandbox2D::OnAttach()
{
	m_SquareVA = Snow::VertexArray::Create();

	float verticesSquare[4 * 5] = {
		-0.5f,-0.5f,0.0f, 0.0f, 0.0f,
		0.5f,-0.5f,0.0f, 1.0f, 0.0f,
		0.5f,0.5f,0.0f, 1.0f, 1.0f,
		-0.5f,0.5f,0.0f, 0.0f, 1.0f
	};

	Snow::Ref<Snow::VertexBuffer> vb(Snow::VertexBuffer::Create(verticesSquare, sizeof(verticesSquare)));

	Snow::BufferLayout Squarelayout =
	{
		{Snow::ShaderDataType::Float3,"a_Position"},
		{Snow::ShaderDataType::FLoat2,"a_TexCoord"}
	};

	vb->SetLayout(Squarelayout);

	m_SquareVA->AddVertexBuffer(vb);

	uint32_t indicesSquare[6] = { 0,1,2,2,3,0 };
	Snow::Ref<Snow::IndexBuffer> ib(Snow::IndexBuffer::Create(indicesSquare, sizeof(indicesSquare) / sizeof(uint32_t)));

	m_SquareVA->SetIndexBuffer(ib);
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Snow::Timestep ts)
{

	m_CameraController.OnUpdate(ts);

	if (Snow::Input::IsKeyPressed(Snow::Key::Left))
	{
		m_SquarePosition.x -= m_speed * ts;
	}
	if (Snow::Input::IsKeyPressed(Snow::Key::Right))
	{
		m_SquarePosition.x += m_speed * ts;
	}
	if (Snow::Input::IsKeyPressed(Snow::Key::Up))
	{
		m_SquarePosition.y += m_speed * ts;
	}
	if (Snow::Input::IsKeyPressed(Snow::Key::Down))
	{
		m_SquarePosition.y -= m_speed * ts;
	}

	Snow::RenderCommand::SetClearColor({ 0.4f, 0.4f, 0.9f, 1.0f });
	Snow::RenderCommand::Clear();

	Snow::Renderer::BeginScene(m_CameraController.GetCamera());

	glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_SquarePosition) * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
	Snow::Ref<Snow::Shader> shader = m_ShaderLib.Get("PlainColor");
	shader->UploadUniformFloat4("u_Color", glm::vec4(0.1f,0.7f,0.3f,1.0f));
	Snow::Renderer::Submit(m_SquareVA, m_ShaderLib.Get("PlainColor"), transform);

	Snow::Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
}

void Sandbox2D::OnEvent(Snow::Event& e)
{
	m_CameraController.OnEvent(e);
}
