#include "bepch.h"
#include "Texture.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Beati {

	Ref<Texture2D> Beati::Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:		BE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:	return std::make_shared<OpenGLTexture2D>(path);
			case RendererAPI::API::Vulkan:	BE_CORE_ASSERT(false, "RendererAPI::Vulkan is currently not supported!"); return nullptr;
			case RendererAPI::API::DirectX:	BE_CORE_ASSERT(false, "RendererAPI::DirectX is currently not supported!"); return nullptr;
		}
		BE_CORE_ASSERT(false, "Unknown RendererAIP")
		return nullptr;
	
	}
}