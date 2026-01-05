#include <Beati.h>
// ---- EntryPonit para windows ----
#include "Beati/Core/EntryPoint.h"
// ---------------------------------

#include "SandboxLayer.h"

class Sandbox2D : public Beati::Application
{
public:
	Sandbox2D()
	{
		PushLayer(new SandboxLayer());
	}

    ~Sandbox2D() {}
};

Beati::Application* Beati::CreateApplication()
{
    return new Sandbox2D();
}