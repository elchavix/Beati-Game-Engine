#include "bepch.h"
#include "Shader.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Beati {
	
	Shader* Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:		BE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:	return new OpenGLShader(filepath);
			case RendererAPI::API::Vulkan:	BE_CORE_ASSERT(false, "RendererAPI::Vulkan is currently not supported!"); return nullptr;
			case RendererAPI::API::DirectX:	BE_CORE_ASSERT(false, "RendererAPI::DirectX is currently not supported!"); return nullptr;
		}
		BE_CORE_ASSERT(false, "Unknown RendererAIP")
			return nullptr;
	}

	Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:		BE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:	return new OpenGLShader(vertexSrc, fragmentSrc);
			case RendererAPI::API::Vulkan:	BE_CORE_ASSERT(false, "RendererAPI::Vulkan is currently not supported!"); return nullptr;
			case RendererAPI::API::DirectX:	BE_CORE_ASSERT(false, "RendererAPI::DirectX is currently not supported!"); return nullptr;
		}
		BE_CORE_ASSERT(false, "Unknown RendererAIP")
		return nullptr;
	}

}