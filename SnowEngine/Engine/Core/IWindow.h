#pragma once

#include "SnowEngine/Engine/Core/Events/Event.h"
#include <functional>

#include <glm/glm.hpp>

namespace Snow
{
	struct SNOW_API WindowProperties
	{
		unsigned int Width;
		unsigned int Height;
		std::string Title;
		bool VSync = true;
		WindowProperties(unsigned int width = 1280, unsigned int height = 720, const std::string& title = "SnowEngine")
			: Width(width), Height(height), Title(title) {
		}
	};

	class SNOW_API IWindow
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~IWindow() = default;
		
		virtual void Init(const WindowProperties& props) = 0;
		
		virtual void OnUpdate() = 0;
		
		virtual void Shutdown() = 0;
		
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;

		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;
		
		static IWindow* Create(const WindowProperties& props = WindowProperties());

	protected:
		
	};
}