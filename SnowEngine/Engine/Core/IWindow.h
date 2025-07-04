#pragma once

#include "SnowEngine/Engine/Core/Events/Event.h"
#include <functional>

#include <glm/glm.hpp>

namespace Snow
{
	struct WindowProperties
	{
		unsigned int Width;
		unsigned int Height;
		std::string Title;
		bool VSync = true;
		WindowProperties(unsigned int width = 1280, unsigned int height = 720, const std::string& title = "SnowEngine")
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

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;
		
		static Ref<IWindow> Create();
		virtual void Init(const WindowProperties& props = WindowProperties()) = 0;
	protected:
		virtual void Shutdown() = 0;
	};
}