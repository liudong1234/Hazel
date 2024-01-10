#include "hzpch.h"
#include "OpenGLTexture2D.h"

#include "stb_image.h"
#include <glad/glad.h>
namespace Hazel
{
	OpenGLTexture2D::OpenGLTexture2D(std::string& filepath)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);

		stbi_uc* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
		HZ_CORE_ASSERT(data, "Texture loads failly!");

		this->m_Height = height;
		this->m_Width = width;

		glCreateTextures(GL_TEXTURE_2D, 1, &this->m_RendererID);
		glTextureStorage2D(this->m_RendererID, 1, GL_RGB8, this->m_Width, this->m_Height);

		glTextureParameteri(this->m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(this->m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureSubImage2D(this->m_RendererID, 0, 0, 0, this->m_Width, this->m_Height, GL_RGB, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);

	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &this->m_RendererID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot)
	{
		glBindTextureUnit(slot, this->m_RendererID);
	}
}