#include <SnowEngine/SnowEngine.h>

class ExampleLayer : public Snow::Layer
{
public:
	ExampleLayer() : Layer("Example") {}

	void OnUpdate() override
	{
		
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
		PushOverlay(new Snow::ImGuiLayer());
	}
	~Sandbox(){}
};

Snow::Application* Snow::CreateApplication()
{
	return new Sandbox();
}