#pragma once

#include "Hazel/Renderer/Texture.h"

namespace Hazel
{
	class OpenGLTexture2D :
		public Texture2D
	{
	public:
		OpenGLTexture2D(std::string filepath);
		virtual ~OpenGLTexture2D();

		virtual void Bind(uint32_t slot = 0) override;
		virtual void UnBind() override;

	private:
		uint32_t m_RendererID;
		//unsigned char* m_LocalBuffer;
		uint32_t m_Width, m_Height;

	};
}