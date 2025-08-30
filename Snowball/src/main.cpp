#include "EditorLayer.h"



class Editor : public Snow::Application
{
public:
	Editor()
		:Application("Snowball")
	{
		PushLayer(new Snow::EditorLayer());
	}

	~Editor() {}
};

#include <SnowEngine/EntryPoint.h>

Snow::Application* Snow::CreateApplication()
{
	return new Editor();
}
