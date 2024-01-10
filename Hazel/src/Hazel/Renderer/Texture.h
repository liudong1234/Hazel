#pragma once


namespace Hazel
{
	class Texture
	{
	public:
		virtual ~Texture() = default;
		virtual void Bind(uint32_t slot = 0) = 0;
		virtual void UnBind() = 0;
	};

	class Texture2D :
		public Texture
	{
	public:
		static Texture2D* Create(std::string filepath);
	};

}