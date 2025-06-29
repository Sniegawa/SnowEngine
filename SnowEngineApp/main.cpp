#include "Sandbox2D.h"

class Sandbox : public Snow::Application
{
public:
	Sandbox()
	{
		PushLayer(new Sandbox2D());
	}

	~Sandbox(){}
};

#include <SnowEngine/EntryPoint.h>

Snow::Application* Snow::CreateApplication()
{
	return new Sandbox();
}

