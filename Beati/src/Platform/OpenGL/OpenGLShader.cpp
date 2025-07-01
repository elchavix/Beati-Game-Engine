#include "bepch.h"
#include "OpenGLShader.h"
#include "Beati/Log.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>



namespace Beati {

	OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		Compile(vertexSrc, fragmentSrc);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::UploadloadUniformInt(const std::string& name, int value)
	{
		GLint location = GetUniformLocation(name);
		if (location == -1)
		{
			BE_CORE_ERROR("Uniform '{0}' not found in shader!", name);
			return;
		}
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadloadUniformFloat(const std::string& name, float value)
	{
		GLint location = GetUniformLocation(name);
		if (location == -1)
		{
			BE_CORE_ERROR("Uniform '{0}' not found in shader!", name);
			return;
		}
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadloadUniformFloat2(const std::string& name, const glm::vec2& vec2f)
	{
		GLint location = GetUniformLocation(name);
		if (location == -1)
		{
			BE_CORE_ERROR("Uniform '{0}' not found in shader!", name);
			return;
		}
		glUniform2f(location, vec2f.x, vec2f.y);
	}
	void OpenGLShader::UploadloadUniformFloat3(const std::string& name, const glm::vec3& vec3f)
	{
		GLint location = GetUniformLocation(name);
		if (location == -1)
		{
			BE_CORE_ERROR("Uniform '{0}' not found in shader!", name);
			return;
		}
		glUniform3f(location, vec3f.x, vec3f.y, vec3f.z);
	}


	void OpenGLShader::UploadloadUniformFloat4(const std::string& name, const glm::vec4& vec4f)
	{
		GLint location = GetUniformLocation(name);
		if (location == -1)
		{
			BE_CORE_ERROR("Uniform '{0}' not found in shader!", name);
			return;
		}
		glUniform4f(location, vec4f.x, vec4f.y, vec4f.z, vec4f.w);
	}

	void OpenGLShader::UploadloadUniformMatrix3f(const std::string& name, const glm::mat3& matrix)
	{
		GLint location = GetUniformLocation(name);
		if (location == -1)
		{
			BE_CORE_ERROR("Uniform '{0}' not found in shader!", name);
			return;
		}
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadloadUniformMatrix4f(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = GetUniformLocation(name);
		if (location == -1)
		{
			BE_CORE_ERROR("Uniform '{0}' not found in shader!", name);
			return;
		}
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::Compile(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		m_RendererID = CreateShader(vertexSrc, fragmentSrc);
	}

	uint32_t OpenGLShader::CreateShader(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		// Create an empty vertex shader handle
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar* source = (const GLchar*)vertexSrc.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		// Compile the vertex shader
		glCompileShader(vertexShader);

		GLint isCompiled = GL_FALSE;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(vertexShader);

			BE_CORE_ERROR("Vertex shader compilation failed: {0}", infoLog.data());
			BE_CORE_ASSERT(false, "Vertex shader compilation failed!");
			return -1;
		}

		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		source = (const GLchar*)fragmentSrc.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		// Compile the fragment shader
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(fragmentShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(vertexShader);

			BE_CORE_ERROR("Fragment shader compilation failed: {0}", infoLog.data());
			BE_CORE_ASSERT(false, "Fragment shader compilation failed!");
			return -1;
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		GLuint program = glCreateProgram();

		// Attach our shaders to our program
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = GL_FALSE;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			// In this simple program, we'll just leave
			BE_CORE_ERROR("Shader Link compilation failed: {0}", infoLog.data());
			BE_CORE_ASSERT(false, "Shader Link compilation failed!");
			return -1;
		}
		// Always detach shaders after a successful link.
		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);

		return program;
	}

	int OpenGLShader::GetUniformLocation(const std::string& name)
	{
		return glGetUniformLocation(m_RendererID, name.c_str());
	}
}