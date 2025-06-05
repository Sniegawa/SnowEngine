#include <SnowEngine/SnowEngine.h>

int main(int argc, char* argv[])
{
	SnowEngine::Application* app = new SnowEngine::Application();
	app->innit();
	
	app->Run();

	delete app;
	return 0;
}