#include "Core.h"

using namespace SnowEngine;

Application::Application()
{
	std::cout << "Application Created" << std::endl;
}

Application::~Application()
{
	//cleanup();
	std::cout << "Application destroyed" << std::endl;
}

void Application::innit()
{
	std::cout << "Application initialized" << std::endl;
	
}

void Application::cleanup()
{
	std::cout << "Application cleaned up" << std::endl;
}

void Application::test()
{
	std::cout << "Test function called" << std::endl;
}

