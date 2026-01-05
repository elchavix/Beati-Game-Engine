#include "bepch.h"
#include "VertexArray.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "Renderer.h"

namespace Beati {

	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	BE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:	return std::make_shared<OpenGLVertexArray>();
			case RendererAPI::API::Vulkan:	BE_CORE_ASSERT(false, "RendererAPI::Vulkan is currently not supported!"); return nullptr;
			case RendererAPI::API::DirectX:	BE_CORE_ASSERT(false, "RendererAPI::DirectX is currently not supported!"); return nullptr;
		}
		BE_CORE_ASSERT(false, "Unknown RendererAIP")
		return nullptr;
	}

}