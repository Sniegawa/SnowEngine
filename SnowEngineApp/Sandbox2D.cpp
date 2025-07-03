#include "Sandbox2D.h"

Sandbox2D::Sandbox2D()
	: Layer("Application Example Layer"),
		m_CameraController(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, (1280.0f / 720.0f), true)
{

	m_Texture = Snow::Texture2D::Create("Assets/Textures/pizza.png");
	m_Texture2 = Snow::Texture2D::Create("Assets/Textures/BurgerBase.png");
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

	//Snow::Renderer2D::DrawRotatedQuad({ 1.0f,1.0f,0.0f }, { 1.0f,1.0f }, 45.0f, glm::vec4(1.0f,0.0f,0.0f,1.0f));
	//Snow::Renderer2D::DrawQuad({ -2.0f,-1.0f,1.1f }, { 1.0f,2.0f }, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	//Snow::Renderer2D::DrawQuad({ 0.1f,-0.25f,1.1f }, { 1.0f,1.0f }, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	//Snow::Renderer2D::DrawQuad({ 0.0f,0.0f,1.0f }, { 1.0f,1.0f }, glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
	m_Texture->SetTextureTint(tint);
	//Snow::Renderer2D::DrawQuad(m_SquarePosition, scale, m_Texture);
	m_Texture->SetTextureTint(glm::vec3(1.0f));
	int maxX = 100, maxY = 100;
	for (int x = 0; x < maxX; x++)
	{
		for (int y = 0; y < maxY; y++)
		{
			if (x == y || x == 0 || y == 0 || x == maxX-1 || y == maxY-1)
			{
				Snow::Renderer2D::DrawQuad({ x,y }, { 0.5f,0.5f }, m_Texture);
			}
			else if (abs(x - y) == 1)
			{
				Snow::Renderer2D::DrawQuad({ x,y }, { 0.5f,0.5f }, m_Texture2);
			}
			else if ((x+y) % 2 == 0)
			{
				Snow::Renderer2D::DrawQuad({ x,y }, { 0.5f,0.5f }, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
			}
			else
			{
				Snow::Renderer2D::DrawQuad({ x,y }, { 0.5f,0.5f }, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
			}
		}
	}

	Snow::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("PIZZA");
	ImGui::DragFloat("Rotation", &rotation, 1.0f, 0.0f, 360.0f);
	ImGui::DragFloat("Z", &m_SquarePosition.z, 0.05f, -2.0f, 2.0f);
	ImGui::DragFloat2("Scale", glm::value_ptr(scale), 0.1f, -1.0f, 10.0f);
	ImGui::ColorPicker3("Tint", glm::value_ptr(tint));
	ImGui::End();
}

void Sandbox2D::OnEvent(Snow::Event& e)
{
	m_CameraController.OnEvent(e);
}
