#include "Core.h"

namespace Snow 
{

#define BIND_EVENT_FN(fn) std::bind(&fn,this,std::placeholders::_1)

	Application::Application()
	{
	}

	Application::~Application()
	{
		SNOW_CORE_INFO("Application destruction");
		delete m_window;
	}



	void Application::innit()
	{
		Log::Init();
		m_window = IWindow::Create(WindowProperties(1280, 720, "SnowEngine"));
		m_window->SetClearColor(glm::vec4(0.1f, 0.1f, 0.7f, 1.0f));
		m_window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
		SNOW_CORE_INFO("Application initialized");
	}


	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}
	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
	}


	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled())
				break;
		}
	}

	void Application::Run()
	{
		SNOW_CORE_INFO("Application running");

		while (!m_ShouldClose)
		{
			m_window->OnUpdate();

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_ShouldClose = true;
		return true;
	}
};