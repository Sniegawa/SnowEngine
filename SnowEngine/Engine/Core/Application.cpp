#include "Application.h"

#include <chrono>

#include "Logging/Log.h"
#include "Renderer/Renderer.h"
#include "Renderer/Renderer2D.h"
#include "Audio/AudioSystem.h"
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
		AudioSystem::Init();
		Renderer::Init();

		PushOverlay(m_ImGuiLayer);

		SNOW_CORE_INFO("Application initialized");
	}

	Application::~Application()
	{
		Renderer2D::Shutdown();
		AudioSystem::Shutdown();
		SNOW_CORE_INFO("Application destruction");
	}

	void Application::PushLayer(Layer* layer)
	{
		SNOW_CORE_INFO("Attaching layer {0}", layer->GetName());
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay)
	{
		SNOW_CORE_INFO("Attaching overlay {0}", overlay->GetName());
		m_LayerStack.PushOverlay(overlay);
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
		auto lastTime = std::chrono::high_resolution_clock::now();
		while (!m_ShouldClose)
		{
			auto now = std::chrono::high_resolution_clock::now();
			float deltaTimeMs = std::chrono::duration<float>(now - lastTime).count();
			lastTime = now;
			Timestep timestep = deltaTimeMs;

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
			AudioSystem::Update(timestep);
		}
	}

	void Application::Close()
	{
		m_ShouldClose = true;
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