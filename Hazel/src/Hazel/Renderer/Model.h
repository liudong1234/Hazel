#pragma once

#include "Shader.h"

namespace Hazel
{
	class Model
	{
	public:
		virtual void Draw(std::shared_ptr<Shader>&) = 0;
	public:
		static std::shared_ptr<Model> Create(const char* filepath);
	};
}