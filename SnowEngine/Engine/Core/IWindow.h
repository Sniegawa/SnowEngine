#pragma once

#include "SnowEngine/Engine/Core/Events/Event.h"
#include <functional>

#include <glm/glm.hpp>

namespace Snow
{
	struct WindowProperties
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;
		bool VSync = true;
		WindowProperties(const std::string& title = "SnowEngine",uint32_t width = 1280, uint32_t height = 720)
			: Width(width), Height(height), Title(title) {
		}
	};

	class IWindow
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~IWindow() = default;
		
		
		virtual void OnUpdate() = 0;

		
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;

		virtual void* GetNativeWindow() = 0;

		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		
		static Ref<IWindow> Create();
		virtual void Init(const WindowProperties& props = WindowProperties()) = 0;
	protected:
		virtual void Shutdown() = 0;
	};
}
