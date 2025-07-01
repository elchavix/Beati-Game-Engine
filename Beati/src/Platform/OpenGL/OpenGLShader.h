#pragma once
#include "Beati/Renderer/Shader.h"
#include <glm/glm.hpp>

namespace Beati {
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		void UploadloadUniformInt(const std::string& name, int value);

		void UploadloadUniformFloat(const std::string& name, float value);
		void UploadloadUniformFloat2(const std::string& name, const glm::vec2& vec2f);
		void UploadloadUniformFloat3(const std::string& name, const glm::vec3& vec3f);
		void UploadloadUniformFloat4(const std::string& name, const glm::vec4& vec4f);

		void UploadloadUniformMatrix3f(const std::string& name, const glm::mat3& matrix);
		void UploadloadUniformMatrix4f(const std::string& name, const glm::mat4& matrix);
	private:
		uint32_t m_RendererID;

		void Compile(const std::string& vertexSrc, const std::string& fragmentSrc);
		uint32_t CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
		int GetUniformLocation(const std::string& name);
	};
}
