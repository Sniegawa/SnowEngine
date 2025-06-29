#pragma once

#include <iostream>
#include <memory>

#include "glm/glm.hpp"

#include "SnowEngineAPI.h"
#include "IWindow.h"

#include "Events/ApplicationEvent.h"

#include "Timestep.h"

#include "Layers/LayerStack.h"
#include "ImGui/ImGuiLayer.h"

namespace Snow
{
	class Application
	{
	public:
		Application();
		~Application();
		void Run();
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline IWindow& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		bool m_ShouldClose = false;
		bool m_Minimized = false;
		Ref<IWindow> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;

		static Application* s_Instance;
	};

	Application* CreateApplication();
};