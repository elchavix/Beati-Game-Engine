#include "bepch.h"
#include "RendererCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Beati {

	Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();
}
