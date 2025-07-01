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

	Snow::Renderer2D::DrawRotatedQuad({ 1.0f,1.0f,0.0f }, { 1.0f,1.0f }, 45.0f, glm::vec4(1.0f,0.0f,0.0f,1.0f));
	Snow::Renderer2D::DrawQuad({ -2.0f,2.5f,1.0f }, { 1.0f,1.0f }, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	Snow::Renderer2D::DrawQuad({ 3.0f,-2.0f,-1.0f }, { 1.0f,1.0f }, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	m_Texture->SetTextureTint(tint);
	Snow::Renderer2D::DrawRotatedQuad(m_SquarePosition, scale, rotation, m_Texture);



	Snow::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("PIZZA");
	ImGui::DragFloat("Rotation", &rotation, 1.0f, 0.0f, 360.0f);
	ImGui::DragFloat2("Scale", glm::value_ptr(scale), 0.1f, -1.0f, 10.0f);
	ImGui::ColorPicker3("Tint", glm::value_ptr(tint));
	ImGui::End();
}

void Sandbox2D::OnEvent(Snow::Event& e)
{
	m_CameraController.OnEvent(e);
}
