#include <SnowEngine/SnowEngine.h>

class ExampleLayer : public Snow::Layer
{
public:
	ExampleLayer() : Layer("Example") {}

	void OnUpdate() override
	{
		if (Snow::Input::IsKeyPressed(Snow::Key::X))
		{
			SNOW_CLIENT_TRACE("X was pressed");
		}
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Test");
		ImGui::Text("Hello WORLD");
		ImGui::End();
	}

	void OnEvent(Snow::Event& event) override
	{
		//SNOW_CLIENT_TRACE("{0}", event.ToString());
	}

};

class Sandbox : public Snow::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox(){}
};

Snow::Application* Snow::CreateApplication()
{
	return new Sandbox();
}
