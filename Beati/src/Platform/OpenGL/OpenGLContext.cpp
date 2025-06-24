#include "bepch.h"
#include "OpenGLContext.h"

#include "Beati/Log.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>


namespace Beati {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		BE_CORE_ASSERT(windowHandle, "Window handle is null!");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent((GLFWwindow*)m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		BE_CORE_ASSERT(status, "Failed to initialize GLAD!");

		BE_CORE_INFO("OpenGL Info:");
		BE_CORE_INFO("    Vendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
		BE_CORE_INFO("    Renderer: {0}",  reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
		BE_CORE_INFO("    Version: {0}",  reinterpret_cast<const char*>(glGetString(GL_VERSION)));
		BE_CORE_INFO("    GLSL Version: {0}", reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers((GLFWwindow*)m_WindowHandle);
	}
}

