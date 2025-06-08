#include <SnowEngine/SnowEngine.h>

class ExampleLayer : public Snow::Layer
{
public:
	ExampleLayer() : Layer("Example") {}

	void OnUpdate() override
	{
		//SNOW_CLIENT_INFO("EXAMPLELAYER::UPDATE");
	}

	void OnEvent(Snow::Event& event) override
	{
		SNOW_CLIENT_TRACE("{0}", event.ToString());
	}

};

class Sandbox : public Snow::Application
{
public:
	Sandbox()
	{
		innit();
		PushLayer(new ExampleLayer());
	}
	~Sandbox(){}
};

Snow::Application* Snow::CreateApplication()
{
	return new Sandbox();
}

//int main(int argc, char* argv[])
//{
//	//Snow::Application* app = new Snow::Application();
//	//app->innit();
//	
//	//app->Run();
//
//	//delete app;
//
//
//	return 0;
//}