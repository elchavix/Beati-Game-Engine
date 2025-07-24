#include "bepch.h"
#include "OpenGLShader.h"

#include <fstream>
#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>



namespace Beati {

	static GLenum ShaderTypeFormString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		BE_CORE_ASSERT(false, "Unknown shader type!");
	}


	OpenGLShader::OpenGLShader(const std::string& filepath)
	{
		std::string source = ReadFile(filepath);

		auto shaderSources = PreProcess(source);

		Compile(shaderSources);

		// Extract the name from the filepath
		m_Name = filepath.substr(filepath.find_last_of("/\\") + 1);
		if (m_Name.find_last_of('.') != std::string::npos)
			m_Name = m_Name.substr(0, m_Name.find_last_of('.')); // Remove file extension
		else
			BE_CORE_WARN("Shader file '{0}' has no extension!", filepath);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
		: m_Name(name)
	{
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(sources);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());

			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());

			in.close();
		}
		else
		{
			BE_CORE_ERROR("Could not open file '{0}'", filepath);
		}
		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			BE_CORE_ASSERT(eol != std::string::npos, "Syntaz error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			BE_CORE_ASSERT(type == "vertex" || type == "fragment" || type == "pixel", "invalid shader type speci");


			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFormString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}
		return shaderSources;
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::SetInt(const std::string& name, int value)
	{
		GLint location = GetUniformLocation(name);
		if (location == -1)
		{
			BE_CORE_ERROR("Uniform '{0}' not found in shader!", name);
			return;
		}
		glUniform1i(location, value);
	}

	void OpenGLShader::SetFloat(const std::string& name, float value)
	{
		GLint location = GetUniformLocation(name);
		if (location == -1)
		{
			BE_CORE_ERROR("Uniform '{0}' not found in shader!", name);
			return;
		}
		glUniform1f(location, value);
	}

	void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& value)
	{
		GLint location = GetUniformLocation(name);
		if (location == -1)
		{
			BE_CORE_ERROR("Uniform '{0}' not found in shader!", name);
			return;
		}
		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		GLint location = GetUniformLocation(name);
		if (location == -1)
		{
			BE_CORE_ERROR("Uniform '{0}' not found in shader!", name);
			return;
		}
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		GLint location = GetUniformLocation(name);
		if (location == -1)
		{
			BE_CORE_ERROR("Uniform '{0}' not found in shader!", name);
			return;
		}
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::SetMat3(const std::string& name, const glm::mat3& matrix)
	{
		GLint location = GetUniformLocation(name);
		if (location == -1)
		{
			BE_CORE_ERROR("Uniform '{0}' not found in shader!", name);
			return;
		}
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = GetUniformLocation(name);
		if (location == -1)
		{
			BE_CORE_ERROR("Uniform '{0}' not found in shader!", name);
			return;
		}
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
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

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSrc)
	{
		GLuint program = glCreateProgram();
		BE_CORE_ASSERT(shaderSrc.size() <= 2, "OpenGL only supports 2 shaders at a time (vertex and fragment).");
		std::array<GLenum, 2> glShaderIDs;
		int shaderIndex = 0;

		for (auto& kv : shaderSrc)
		{
			GLenum type = kv.first;
			const std::string& source = kv.second;
			 
			GLuint shader = glCreateShader(type);

			const GLchar* sourceCstr = (const GLchar*)source.c_str();
			glShaderSource(shader, 1, &sourceCstr, 0);

			glCompileShader(shader);

			GLint isCompiled = GL_FALSE;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				BE_CORE_ERROR("Shader compilation failed (type {}):\n{}", type, infoLog.data());
				BE_CORE_ASSERT(false, "Shader compilation failed!");
				return;
			}
			glAttachShader(program, shader);
			glShaderIDs[shaderIndex++] = shader;
		}
		
		glLinkProgram(program);

		GLint isLinked = GL_FALSE;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(program);
			for (auto id : glShaderIDs)
				glDeleteShader(id);

			BE_CORE_ERROR("Shader Link compilation failed: {0}", infoLog.data());
			BE_CORE_ASSERT(false, "Shader Link compilation failed!");
			return;
		}
		for (auto id : glShaderIDs)
			glDetachShader(program, id);

		m_RendererID = program;
	}

	int OpenGLShader::GetUniformLocation(const std::string& name)
	{
		return glGetUniformLocation(m_RendererID, name.c_str());
	}
}