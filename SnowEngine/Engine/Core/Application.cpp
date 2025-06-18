#include "Application.h"

namespace Snow 
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		SNOW_ASSERT(!s_Instance, "App already exists!");
		s_Instance = this;
		Log::Init();
		m_Window = IWindow::Create(WindowProperties(1280, 720, "SnowEngine"));
		m_Window->SetClearColor(glm::vec4(0.1f, 0.1f, 0.7f, 1.0f));
		m_Window->SetEventCallback(SNOW_BIND_EVENT_FN(Application::OnEvent,1));

		m_ImGuiLayer = new ImGuiLayer();
		
		PushOverlay(m_ImGuiLayer);

		SNOW_CORE_INFO("Application initialized");
	}

	Application::~Application()
	{
		delete m_Window;
		SNOW_CORE_INFO("Application destruction");
	}

	void Application::PushLayer(Layer* layer)
	{
		SNOW_CORE_INFO("Attaching layer {0}", layer->GetName());
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();

	}

	void Application::PushOverlay(Layer* overlay)
	{
		SNOW_CORE_INFO("Attaching overlay {0}", overlay->GetName());
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(SNOW_BIND_EVENT_FN(Application::OnWindowClose,1));

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
			m_Window->ClearWindow();
			
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_ImGuiLayer->Begin();

			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();

			m_ImGuiLayer->End();
			m_Window->OnUpdate();
			
			if (Input::IsKeyPressed(Key::A))
			{
				auto [x, y] = Input::GetMousePosition();

				SNOW_CORE_TRACE("{0},{1}", x, y);
			}
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_ShouldClose = true;
		return true;
	}
};