#include "Core.h"

using namespace SnowEngine;

Application::Application()
{
}

Application::~Application()
{
	//cleanup();
	std::cout << "Application destroyed" << std::endl;
}

void Application::innit()
{
	Log::Init();
}

void Application::cleanup()
{
	std::cout << "Application cleaned up" << std::endl;
}

void Application::test()
{

}

