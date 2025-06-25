#include "bepch.h"
#include "VertexArray.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "Renderer.h"

namespace Beati {

	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetRendererAPI())
		{
			case RendererAPI::None:		BE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::OpenGL:	return new OpenGLVertexArray();
			case RendererAPI::Vulkan:	BE_CORE_ASSERT(false, "RendererAPI::Vulkan is currently not supported!"); return nullptr;
			case RendererAPI::DirectX:	BE_CORE_ASSERT(false, "RendererAPI::DirectX is currently not supported!"); return nullptr;
		}
		return nullptr;
	}

}