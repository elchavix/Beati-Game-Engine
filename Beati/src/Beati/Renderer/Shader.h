#pragma once
#include <string>


namespace Beati {
	class Shader
	{
	public:
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();

		void Bind();
		void Unbind();
	private:
		uint32_t m_RendererID;

		void Compile(const std::string& vertexSrc, const std::string& fragmentSrc);
		uint32_t CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
		int GetUniformLocation(const std::string& name);
	};
}
