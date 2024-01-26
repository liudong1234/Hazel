#pragma once

#include "Hazel/Renderer/Texture.h"

namespace Hazel
{
	class OpenGLTexture2D :
		public Texture2D
	{
	public:
		OpenGLTexture2D(std::string filepath);
		OpenGLTexture2D(std::string filepath, std::string type);
		virtual ~OpenGLTexture2D();

		virtual void Bind(uint32_t slot = 0) override;
		virtual void UnBind() override;

		static void ActivateTexture(uint32_t slot = 0);
		inline std::string GetType() { return this->m_Type; }
		inline std::string GetPath() { return this->m_Path; }
	private:
		uint32_t m_RendererID;
		//unsigned char* m_LocalBuffer;
		uint32_t m_Width, m_Height;
		std::string m_Type;
		std::string m_Path;
	};
}