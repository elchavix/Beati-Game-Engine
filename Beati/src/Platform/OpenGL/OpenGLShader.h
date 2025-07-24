#pragma once
#include "Beati/Renderer/Shader.h"

#include <glm/glm.hpp>

// Temporary. TODO: Remove this
typedef unsigned int GLenum;

namespace Beati {
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();


		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetInt(const std::string& name, int value) override;
		virtual void SetFloat(const std::string& name, float value) override;
		virtual void SetFloat2(const std::string& name, const glm::vec2& value) override;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) override;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) override;
		virtual void SetMat3(const std::string& name, const glm::mat3& matrix) override;
		virtual void SetMat4(const std::string& name, const glm::mat4& matrix) override;

		virtual const std::string& GetName() const override { return m_Name; };

		void UploadloadUniformInt(const std::string& name, int value);

		void UploadloadUniformFloat(const std::string& name, float value);
		void UploadloadUniformFloat2(const std::string& name, const glm::vec2& vec2f);
		void UploadloadUniformFloat3(const std::string& name, const glm::vec3& vec3f);
		void UploadloadUniformFloat4(const std::string& name, const glm::vec4& vec4f);

		void UploadloadUniformMatrix3f(const std::string& name, const glm::mat3& matrix);
		void UploadloadUniformMatrix4f(const std::string& name, const glm::mat4& matrix);
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
	private:
		void Compile(const std::unordered_map<GLenum, std::string>& shaderSrc);

		int GetUniformLocation(const std::string& name);

	private:
		uint32_t m_RendererID;
		std::string m_Name;
	};
}
