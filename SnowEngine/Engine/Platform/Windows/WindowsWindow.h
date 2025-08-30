#pragma once

#include "SnowEngine/Engine/Core/IWindow.h"
#include "SnowEngine/Engine/SnowEngineAPI.h"

#include "SnowEngine/Engine/Core/Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>


namespace Snow
{
	class WindowsWindow : public IWindow
	{
	public:
		WindowsWindow();
		
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline uint32_t GetWidth() const override { return m_Data.Width; }
		inline uint32_t GetHeight() const override { return m_Data.Height; }

		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;
		
		void* GetNativeWindow() override { return m_Window; }

		void Init(const WindowProperties& props) override;
	protected:
		void Shutdown() override;

	private:
		
		Scope<GraphicsContext> m_Context;
		GLFWwindow* m_Window;

		struct WindowData
		{
			std::string Title;
			uint32_t Width;
			uint32_t Height;
			bool VSync;
			EventCallbackFn EventCallback;

		};
		
		WindowData m_Data;
		
	};
};