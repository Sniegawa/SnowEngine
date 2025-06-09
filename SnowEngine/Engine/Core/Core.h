#pragma once

#include <iostream>
#include <memory>

#include "glm/glm.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "../SnowEngineAPI.h"
#include "Logging/Log.h"
#include "Events/ApplicationEvent.h"

#include "IWindow.h"

#include "Layers/LayerStack.h"

namespace Snow
{
	class SNOW_API Application
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
		bool m_ShouldClose = false;
		IWindow* m_Window;
		LayerStack m_LayerStack;
		static Application* s_Instance;
	};

	Application* CreateApplication();
};