#pragma once

#include "Engine/Core/Application.h"

#ifdef SNOW_WINDOWS

extern Snow::Application* Snow::CreateApplication();

int main(int argc, char** argv)
{
	auto app = Snow::CreateApplication();
	app->Run();
	delete app;
}

#endif