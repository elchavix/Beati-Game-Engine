#pragma once
#include "Beati/Layer.h"

namespace Beati 
{
	class BEATI_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		virtual ~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate() override;
		virtual void OnEvent(Event& event) override;

		void Begin();
		void End();

		static ImGuiLayer* Get() { return s_Instance; }
	private:
		static ImGuiLayer* s_Instance;
		float m_time = 0.0f;
		ImGuiLayer(const ImGuiLayer&) = delete;
		ImGuiLayer& operator=(const ImGuiLayer&) = delete;
		ImGuiLayer(ImGuiLayer&&) = delete;
		ImGuiLayer& operator=(ImGuiLayer&&) = delete;
	};
}

