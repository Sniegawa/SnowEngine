#include "Application.h"

#include "Logging/Log.h"

#include <GLFW/glfw3.h>
#include "Renderer/Renderer.h"

namespace Snow 
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		SNOW_ASSERT(!s_Instance, "App already exists!");
		s_Instance = this;

		Log::Init();

		m_Window = IWindow::Create();
		m_Window->Init(WindowProperties(1280, 720, "SnowEngine"));
		m_Window->SetEventCallback(SNOW_BIND_EVENT_FN(Application::OnEvent, 1));

		m_ImGuiLayer = new ImGuiLayer();

		Renderer::Init();

		PushOverlay(m_ImGuiLayer);

		SNOW_CORE_INFO("Application initialized");
	}

	Application::~Application()
	{
		
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
		dispatcher.Dispatch<WindowResizeEvent>(SNOW_BIND_EVENT_FN(Application::OnWindowResize, 1));
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
			auto time = (float)glfwGetTime(); // Temp Platform::GetTime()
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized)
			{
				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(timestep);
			}

			m_ImGuiLayer->Begin();

			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();

			m_ImGuiLayer->End();
			m_Window->OnUpdate();
		
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_ShouldClose = true;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}
		
		m_Minimized = false;

		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
		return false;
	}
};