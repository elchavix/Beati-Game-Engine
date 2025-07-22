#pragma once
#include <string>
#include <unordered_map>
#include "Beati/Core/Core.h"

namespace Beati {
	class Shader
	{
	public:
  		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const std::string& GetName() const = 0;

		static Ref<Shader> Create(const std::string& filepath);
		static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
	};

	class ShaderLibrary
	{
	public:
		void Add(const std::string& name, const Ref<Shader>& shader);

		void Add(const Ref<Shader>& shader);

		Ref<Shader> Load(const std::string& filepath); // By default "Texture.glsl"

		Ref<Shader> Load(const std::string& name, const std::string& filepath);

		inline bool Exists(const std::string& name) const { return m_Shaders.find(name) != m_Shaders.end(); }

		Ref<Shader> Get(const std::string& name);

	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};
}
