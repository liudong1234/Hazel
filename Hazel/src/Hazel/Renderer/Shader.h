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

		static Shader* Create(std::string& filepath);
		static Shader* Create(std::string& vertexSrc, std::string& fragmentSrc);
	};
}

