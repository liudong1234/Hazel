#pragma once

#include <string>

namespace Hazel
{
	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual void Bind(uint32_t slot = 0) = 0;
	};

	class Texture2D :
		public Texture
	{
	public:
		static Texture2D* Create(std::string& filepath);
	};

}
