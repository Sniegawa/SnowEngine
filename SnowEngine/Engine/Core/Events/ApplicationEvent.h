#pragma once

#include "SnowEngine/Engine/Core/Events/Event.h"

namespace SnowEngine
{
	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: m_Width(width), m_Height(height) {
		}

		inline unsigned int GetWidth() const { return m_Width; }
		inline unsigned int GetHeight() const { return m_Height; }
		std::string ToString() const override
		{
			return "WindowResizeEvent: " + std::to_string(m_Width) + ", " + std::to_string(m_Height);
		}
		EVENT_CLASS_TYPE(WindowResize);
		EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APP|EVENT_CATEGORY_WINDOW);

	private:
		unsigned int m_Width, m_Height;
	};

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() = default;
		std::string ToString() const override
		{
			return "WindowCloseEvent";
		}
		EVENT_CLASS_TYPE(WindowClose);
		EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APP | EVENT_CATEGORY_WINDOW);
	};

	class AppTickEvent : public Event
	{
	public:
		AppTickEvent() = default;
		std::string ToString() const override
		{
			return "AppTickEvent";
		}
		EVENT_CLASS_TYPE(AppTick);
		EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APP);
	};

	class AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() = default;
		std::string ToString() const override
		{
			return "AppUpdateEvent";
		}
		EVENT_CLASS_TYPE(AppUpdate);
		EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APP);
	};
	class AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() = default;
		std::string ToString() const override
		{
			return "AppRenderEvent";
		}
		EVENT_CLASS_TYPE(AppReder);
		EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APP);
	};
};