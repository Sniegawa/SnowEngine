#pragma once

#include "SnowEngine/Engine/Core/IWindow.h"
#include "SnowEngine/Engine/SnowEngineAPI.h"

//#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Snow
{
	class WindowsWindow : public IWindow
	{
	public:
		WindowsWindow(const WindowProperties& props);
		
		virtual ~WindowsWindow();

		void OnUpdate() override;

		void ClearWindow() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;
		
		void SetClearColor(const glm::vec4& color) override;

		void* GetNativeWindow() override { return m_Window; }

	private:
		void Init(const WindowProperties& props) override;
		void Shutdown() override;
		
		GLFWwindow* m_Window;

		struct WindowData
		{
			std::string Title;
			unsigned int Width;
			unsigned int Height;
			bool VSync;
			EventCallbackFn EventCallback;

		};
		
		WindowData m_Data;
		
	};
};