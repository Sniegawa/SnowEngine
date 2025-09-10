#include "Sandbox2D.h"


Sandbox2D::Sandbox2D()
	: Layer("Application Example Layer"),
		m_CameraController(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, (1280.0f / 720.0f), true)
{

	m_Texture = Snow::Texture2D::Create("Assets/Textures/pizza.png");
	m_Texture2 = Snow::Texture2D::Create("Assets/Textures/BurgerBase.png");
	m_SpriteSheet = Snow::Spritesheet::CreateFromPath("Assets/Spritesheet/RPGpack_sheet_2X.png", glm::vec2(128.0f));
	m_Grass = Snow::Subtexture2D::CreateFromCoords(m_SpriteSheet, { 2,1 },{1,2});

	Snow::SoundConfig config;
	config.volume = 0.15f;

	Snow::AudioSystem::LoadSound("PickupCoin", "Assets/Sounds/pickupCoin.wav");
	Snow::AudioSystem::LoadSound("PickupCoin2", "Assets/Sounds/pickupCoin.wav",config);
	Snow::AudioSystem::LoadMusic("MusicTest", "Assets/Sounds/musicTest.mp3");
}

Sandbox2D::~Sandbox2D()
{

}

void Sandbox2D::OnAttach()
{
	Snow::MusicConfig config;
	config.pitch = 1.0f;
	config.volume = 0.125f;
	config.looping = true;
	Snow::AudioSystem::MusicPlay("MusicTest",config);
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Snow::Timestep ts)
{
	Snow::Renderer2D::ResetStats();
	m_testRotation += ts*10.0f;
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
	if (Snow::Input::IsKeyPressed(Snow::Key::Minus))
	{
		Snow::AudioSystem::SoundPlay("PickupCoin");
	}
	if (Snow::Input::IsKeyPressed(Snow::Key::Equal))
	{
		Snow::AudioSystem::SoundPlay("PickupCoin2");
	}
	if (Snow::Input::IsKeyPressed(Snow::Key::D0))
	{
		Snow::SoundConfig config;
		config.pitch = 0.5f;
		Snow::AudioSystem::SoundPlay("PickupCoin",config);
	}
	Snow::RenderCommand::SetClearColor({ 0.4f, 0.4f, 0.9f, 1.0f });
	Snow::RenderCommand::Clear();
	Snow::Renderer2D::BeginScene(m_CameraController.GetCamera());



	//Snow::Renderer2D::DrawQuad({ 0,0 }, { 1,2 }, m_Grass);

	m_Texture->SetTextureTint(tint);
	//Snow::Renderer2D::DrawRotatedQuad(m_SquarePosition, scale, rotation, m_Texture);
	//TODO : fix Transparency doesn't work properly if object is submitted before something that will be below
	//Possible fix, store each command in batch of draw commands, and then dispatch them at the end

	//multiple objects demo faster in release mode
	

	Snow::Renderer2D::DrawQuad({ 0.0f,0.0f }, { 0.5f,0.5f }, { 1.0f,0.0f,0.0f,1.0f });
	Snow::Renderer2D::DrawRotatedQuad({ 0.0f,0.0f }, { 0.5f,0.5f },1.0f ,{ 0.0f,1.0f,0.0f,1.0f });
	glm::vec3 tintzero(1.0f);
	m_Texture->SetTextureTint(tintzero);

	int maxX = 10, maxY = 10;
	for (int x = 0; x < maxX; x++)
	{
		for (int y = 0; y < maxY; y++)
		{
			if (x == y || x == 0 || y == 0 || x == maxX-1 || y == maxY-1)
			{
				Snow::Renderer2D::DrawRotatedQuad({ x,y }, { 0.5f,0.5f }, m_testRotation, m_Texture);
			}
			else if (abs(x - y) == 1)
			{
				Snow::Renderer2D::DrawQuad({ x,y }, { 0.5f,0.5f }, m_Texture2);
			}
			else if ((x+y) % 2 == 0)
			{
				Snow::Renderer2D::DrawRotatedQuad({ x,y }, glm::vec2((glm::sin(m_testRotation*0.25f)+1.0f)*0.5f), m_testRotation * 5.0f, glm::vec4(1.0f, 0.0f, 0.0f, 0.25f));
			}
			else
			{
				Snow::Renderer2D::DrawRotatedQuad({ x,y }, { 0.5f,0.5f }, -m_testRotation * 5.0f, glm::vec4(0.0f, 0.0f, 1.0f, 0.25f));
			}
		}
	}
	//SNOW_CLIENT_TRACE("MS:{0} (fps:{1})", ts.GetMilliseconds(),1000.0f/ts.GetMilliseconds());

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
	auto stats = Snow::Renderer2D::GetStats();
	std::stringstream Drawcals;
	Drawcals << "Drawcalls : " << stats.DrawCalls << std::endl;
	std::stringstream Quads;
	Drawcals << "Quads : " << stats.QuadCount << std::endl;
	std::stringstream Vertices;
	Drawcals << "Vertices : " << stats.GetTotalVertexCount();
	ImGui::Begin("Renderer2D Stats");
	ImGui::Text(Drawcals.str().c_str());
	ImGui::Text(Quads.str().c_str());
	ImGui::Text(Vertices.str().c_str());
	ImGui::End();
}

void Sandbox2D::OnEvent(Snow::Event& e)
{
	m_CameraController.OnEvent(e);
}
