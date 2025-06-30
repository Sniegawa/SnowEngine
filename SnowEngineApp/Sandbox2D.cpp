#include "Sandbox2D.h"

Sandbox2D::Sandbox2D()
	: Layer("Application Example Layer"),
		m_CameraController(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, (1280.0f / 720.0f), true)
{

	m_Texture = Snow::Texture2D::Create("Assets/Textures/pizza.png");
}

Sandbox2D::~Sandbox2D()
{

}

void Sandbox2D::OnAttach()
{
	
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

	Snow::Renderer2D::BeginScene(m_CameraController.GetCamera());

	//Depth isn't working properly -1.0f z isn't even rendered
	Snow::Renderer2D::DrawQuad({ 1.0f,1.0f,0.0f }, { 1.0f,1.0f }, 45.0f, glm::vec4(1.0f,0.0f,0.0f,1.0f));
	Snow::Renderer2D::DrawQuad({ -2.0f,2.5f,1.0f }, { 1.0f,1.0f }, 45.0f, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	Snow::Renderer2D::DrawQuad({ 3.0f,-2.0f,-1.0f }, { 1.0f,1.0f }, 45.0f, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

	Snow::Renderer2D::DrawQuad(m_SquarePosition, glm::vec2(5.0f,5.0f), rotation, m_Texture);



	Snow::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Rotator");
	ImGui::DragFloat("ROTATE", &rotation, 1.0f, 0.0f, 360.0f);
	ImGui::End();
}

void Sandbox2D::OnEvent(Snow::Event& e)
{
	m_CameraController.OnEvent(e);
}
