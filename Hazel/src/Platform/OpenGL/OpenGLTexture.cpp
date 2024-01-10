#include "hzpch.h"
#include "OpenGLTexture.h"
#include "stb_image.h"

#include <glad/glad.h>

namespace Hazel
{
	OpenGLTexture2D::OpenGLTexture2D(std::string filepath)
	{
		//glGenTextures(1, &this->m_TextureID);
		glCreateTextures(GL_TEXTURE_2D, 1, &this->m_RendererID);
		int width, height, channels;
		//上下翻转,opengl以左下角为（0， 0）点，相当于从下往上
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* m_LocalBuffer = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
		this->m_Width = width;
		this->m_Height = height;

		glTextureStorage2D(this->m_RendererID, 1, GL_RGBA8, this->m_Width, this->m_Width);

		glTextureParameteri(this->m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(this->m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(this->m_RendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(this->m_RendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
		glTextureSubImage2D(this->m_RendererID, 0, 0, 0, this->m_Width, this->m_Height, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);

		HZ_ASSERT(m_LocalBuffer, "加载纹理失败");

		if (m_LocalBuffer)
			stbi_image_free(m_LocalBuffer);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &this->m_RendererID);
	}
	
	void OpenGLTexture2D::Bind(uint32_t slot)
	{
		glBindTextureUnit(slot, this->m_RendererID);
	}
	
	void OpenGLTexture2D::UnBind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

