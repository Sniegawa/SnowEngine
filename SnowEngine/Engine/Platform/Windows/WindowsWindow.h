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

		virtual void OnUpdate() override;

		virtual void SetIcon(const std::string& iconPath) override;

		virtual inline uint32_t GetWidth() const override { return m_Data.Width; }
		virtual inline uint32_t GetHeight() const override { return m_Data.Height; }

		virtual inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		
		virtual void SetVSync(bool enabled) override;
		virtual bool IsVSync() const override;
		
		virtual void* GetNativeWindow() override { return m_Window; }

		virtual void Init(const WindowProperties& props) override;
	protected:
		virtual void Shutdown() override;

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