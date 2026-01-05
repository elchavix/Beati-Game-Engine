#pragma once
#include "Beati/Core/Layer.h"

#include "Beati/Events/ApplicationEvent.h"
#include "Beati/Events/KeyEvent.h"
#include "Beati/Events/MouseEvent.h"

namespace Beati 
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& event) override;

		void Begin();
		void End();

		inline void BlockEvents(bool block) { m_BlockEvents = block; }

		void SetDarkThemeColors();
	private:
		bool m_BlockEvents = true;
	};
}

