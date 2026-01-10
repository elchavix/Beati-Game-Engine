#include <Beati.h>
// ---- EntryPonit para windows ----
#include "Beati/Core/EntryPoint.h"
// ---------------------------------

#include "PhysicsLayer.h"

class Physics : public Beati::Application
{
public:
	Physics()
	{
		PushLayer(new PhysicsLayer());
	}

    ~Physics() {}
};

Beati::Application* Beati::CreateApplication()
{
    return new Physics();
}