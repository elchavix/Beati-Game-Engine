#pragma once

#include "Beati/Events/Event.h"

#include <sstream>

namespace Beati {

	class BEATI_API ApplicationEvent : public Event
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

	class BEATI_API AppTickEvent : public ApplicationEvent
	{
	public:
		AppTickEvent() {}

		EVENT_CLASS_TYPE(AppTick);
	};

	class BEATI_API AppUpdateEvent : public ApplicationEvent
	{
	public:
		AppUpdateEvent() {}

		EVENT_CLASS_TYPE(AppUpdate);
	};

	class BEATI_API AppRenderEvent : public ApplicationEvent
	{
	public:
		AppRenderEvent() {}

		EVENT_CLASS_TYPE(AppRender);
	};

	class BEATI_API WindowResizeEvent : public ApplicationEvent
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

	class BEATI_API WindowCloseEvent : public ApplicationEvent
	{
	public:
		WindowCloseEvent() {}

		EVENT_CLASS_TYPE(WindowClose);
	};
}