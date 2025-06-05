#pragma once

#include "SnowEngine/Engine/Core/IWindow.h"

#include <GLFW/glfw3.h>

namespace SnowEngine
{
	class WindowsWindow : public IWindow
	{
	public:
		WindowsWindow(const WindowProperties& props);
		
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		void SetClearColor(const glm::vec4& color) override;
		bool IsVSync() const override;
		bool ShouldClose() override;

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