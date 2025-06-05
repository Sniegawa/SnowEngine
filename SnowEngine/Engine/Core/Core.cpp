#include "Core.h"

using namespace SnowEngine;

Application::Application()
{

}

Application::~Application()
{
	//cleanup();
}

void Application::innit()
{
	Log::Init();
	SNOW_CORE_INFO("Application initialized");
}

void Application::cleanup()
{
	SNOW_CORE_INFO("Application cleaning");
	
}

void Application::test()
{
	SNOW_CORE_INFO("Test function called");


}

