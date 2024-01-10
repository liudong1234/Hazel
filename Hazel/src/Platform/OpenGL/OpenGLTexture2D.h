#pragma once
#include "Hazel/Renderer/Texture.h"

namespace Hazel
{
	class OpenGLTexture2D :
		public Texture2D
	{
	public:
		OpenGLTexture2D(std::string& filepath);
		virtual ~OpenGLTexture2D();

		virtual void Bind(uint32_t slot = 0) override;

		uint32_t GetWidth() { return this->m_Width; }
		uint32_t GetHeight() { return this->m_Height; }

	private:
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
	};
}