#include <Beati.h>
// ---- EntryPonit para windows ----
#include "Beati/Core/EntryPoint.h"
// ---------------------------------

#include "MondongoLayer.h"

namespace Beati
{
	class Mondongo : public Application
	{
	public:
		Mondongo()
		{
			PushLayer(new MondongoLayer());
		}

		~Mondongo() {}
	};

	Application* CreateApplication()
	{
		return new Mondongo();
	}
}
