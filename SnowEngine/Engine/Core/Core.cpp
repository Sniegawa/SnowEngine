#include "Core.h"

namespace SnowEngine 
{

	Application::Application()
	{

	}

	Application::~Application()
	{
		SNOW_CORE_INFO("Application destruction");
	}



	void Application::innit()
	{
		Log::Init();
		m_window = IWindow::Create(WindowProperties(1280, 720, "SnowEngine"));
		m_window->SetVSync(true);
		m_window->SetClearColor(glm::vec4(0.1f, 0.1f, 0.7f, 1.0f));
		m_window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
		SNOW_CORE_INFO("Application initialized");
	}

	void Application::OnEvent(Event& e)
	{
		SNOW_CORE_INFO("{0}", e.ToString());
	}

	void Application::Run()
	{
		SNOW_CORE_INFO("Application running");

		while (!m_window->ShouldClose())
		{
			m_window->OnUpdate();
		}
	}
};