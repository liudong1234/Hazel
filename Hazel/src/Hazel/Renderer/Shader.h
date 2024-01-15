#pragma once

#include <string>

#include <glm/glm.hpp>

namespace Hazel
{
	class Shader
	{
	public:
		virtual ~Shader() = default;
		virtual void Bind() = 0;
		virtual void UnBind() = 0;
		virtual const std::string& GetName() const = 0;

		static Ref<Shader> Create(const std::string& filepath);
		static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		
	
	};

	class ShaderLibray
	{
	public:
		void Add(const std::string& name, const Ref<Shader>& shader);
		void Add(const Ref<Shader>& shader);
		Ref<Shader> Load(const std::string& filepath);
		Ref<Shader> Load(const std::string& name, const std::string& filepath);

		Ref<Shader> Get(const std::string& name);
		bool Exist(const std::string& name) const;
	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;

	};

}

