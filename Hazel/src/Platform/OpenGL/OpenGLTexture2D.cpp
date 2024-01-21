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

		GLenum internalFormat = 0, dataFormat = 0;

		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}
		HZ_CORE_ASSERT(internalFormat & dataFormat, "texuture format doesn't support");

		glCreateTextures(GL_TEXTURE_2D, 1, &this->m_RendererID);
		glTextureStorage2D(this->m_RendererID, 1, internalFormat, this->m_Width, this->m_Height);

		glTextureParameteri(this->m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(this->m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureSubImage2D(this->m_RendererID, 0, 0, 0, this->m_Width, this->m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

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