#include <Beati.h>

class EjemploCapa : public Beati::Layer
{
public:
	EjemploCapa() : Layer("EjemploCapa")
	{

	}

	void OnAttach() override
	{
		BE_CORE_INFO("EjemploCapa adjuntada");
	}

	void OnDetach() override
	{
		BE_CORE_INFO("EjemploCapa desadjuntada");
	}

	void OnUpdate() override
	{
		//BE_CORE_INFO("EjemploCapa actualizada"); 
		if (Beati::Input::IsMouseButtonPressed(BE_MOUSE_BUTTON_LEFT))
		{
			BE_CORE_INFO("Click izquierdo precionado");
		}
	}

	void OnEvent(Beati::Event& event) override
	{
		// BE_CORE_TRACE("Evento en EjemploCapa: {0}", event.ToString());
	}
};


class Sandbox : public Beati::Application
{
public:
	Sandbox()
	{
		PushLayer(new EjemploCapa());
	}

    ~Sandbox() {}
};

Beati::Application* Beati::CreateApplication()
{
    return new Sandbox();
}

