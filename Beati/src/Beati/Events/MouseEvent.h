#pragma once

#include "Event.h"


namespace Beati {

	class MouseEvent : public Event
	{
	public:
		inline float GetX() const { return m_X; }
		inline float GetY() const { return m_Y; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput);

	protected:
		MouseEvent(float x, float y)
			: m_X(x), m_Y(y) {}

		float m_X, m_Y;
	};

	class MouseMovedEvent : public MouseEvent
	{
	public:
		MouseMovedEvent(float x, float y)
			: MouseEvent(x, y) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_X << ", " << m_Y;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved);
	};

	class MouseScrolledEvent : public MouseEvent
	{
	public:
		MouseScrolledEvent(float xOffset, float yOffset)
			: MouseEvent(0.0f, 0.0f), m_XOffset(xOffset), m_YOffset(yOffset) {}

		inline float GetXOffset() const { return m_XOffset; }
		inline float GetYOffset() const { return m_YOffset; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << m_XOffset << ", " << m_YOffset;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled);

	private:
		float m_XOffset, m_YOffset;
	};

	class MouseButtonEvent : public MouseEvent
	{
	public:
		inline int GetMouseButton() const { return m_MouseButton; }

		EVENT_CLASS_CATEGORY(EventCategoryMouseButton | EventCategoryMouse | EventCategoryInput);

	protected:
		MouseButtonEvent(int mouseButton, float x, float y)
			: MouseEvent(x, y), m_MouseButton(mouseButton) {}
		int m_MouseButton;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int mouseButton, float x, float y)
			: MouseButtonEvent(mouseButton, x, y) {}

		MouseButtonPressedEvent(int mouseButton)
			: MouseButtonEvent(mouseButton, 0.f, 0.f) {} // BORRAR: agregar la posicion del mouse en el constructor y elminiar este.

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_MouseButton << " at (" << m_X << ", " << m_Y << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed);
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int mouseButton, float x, float y)
			: MouseButtonEvent(mouseButton, x, y) {}
		MouseButtonReleasedEvent(int mouseButton)
			: MouseButtonEvent(mouseButton, 0.f, 0.f) {} // BORRAR: agregar la posicion del mouse en el constructor y elminiar este.

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_MouseButton << " at (" << m_X << ", " << m_Y << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased);
	};
}