#pragma once

#include "Event.h"


namespace Beati {

	class ApplicationEvent : public Event
	{
	public:
		EVENT_CLASS_CATEGORY(EventCategoryApplication);

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << GetName() << " Event";
			return ss.str();
		}
	};

	class AppTickEvent : public ApplicationEvent
	{
	public:
		AppTickEvent() {}

		EVENT_CLASS_TYPE(AppTick);
	};

	class AppUpdateEvent : public ApplicationEvent
	{
	public:
		AppUpdateEvent() {}

		EVENT_CLASS_TYPE(AppUpdate);
	};

	class AppRenderEvent : public ApplicationEvent
	{
	public:
		AppRenderEvent() {}

		EVENT_CLASS_TYPE(AppRender);
	};

	class WindowResizeEvent : public ApplicationEvent
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: m_Width(width), m_Height(height) {}

		inline unsigned int GetWidth() const { return m_Width; }
		inline unsigned int GetHeight() const { return m_Height; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize);

	private:
		unsigned int m_Width, m_Height;
	};

	class WindowCloseEvent : public ApplicationEvent
	{
	public:
		WindowCloseEvent() {}

		EVENT_CLASS_TYPE(WindowClose);
	};
}
