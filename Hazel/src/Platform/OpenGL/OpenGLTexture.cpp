#include "hzpch.h"
#include "OpenGLTexture.h"
#include "stb_image.h"

#include <glad/glad.h>

namespace Hazel
{

	namespace Utils
	{
		static GLenum HazelImageFormatToDataFormat()
		{

		}
	}

	OpenGLTexture2D::OpenGLTexture2D(std::string filepath):
		m_Type(""),
		m_Path(filepath),
		m_DataFormat(0), m_InternalFormat(0)
	{
		HZ_PROFILE_FUNCTION();

		int width, height, channels;
		//上下翻转,opengl以左下角为（0， 0）点，相当于从下往上
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* m_LocalBuffer = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
		HZ_ASSERT(m_LocalBuffer, "加载纹理失败");
		
		this->m_Width = width;
		this->m_Height = height;

		switch (channels)
		{
		case 3:
			this->m_InternalFormat = GL_RGB8;
			this->m_DataFormat = GL_RGB;
			break;
		case 4:
			this->m_InternalFormat = GL_RGBA8;
			this->m_DataFormat = GL_RGBA;
			break;
		default:
			HZ_CORE_ASSERT(0, "Dont't support this picture type");
			break;
		}

		glCreateTextures(GL_TEXTURE_2D, 1, &this->m_RendererID);
		glTextureStorage2D(this->m_RendererID, 1, this->m_InternalFormat, this->m_Width, this->m_Height);

		glTextureParameteri(this->m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(this->m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(this->m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(this->m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
		
		glTextureSubImage2D(this->m_RendererID, 0, 0, 0, this->m_Width, this->m_Height, this->m_DataFormat, GL_UNSIGNED_BYTE, m_LocalBuffer);


		if (m_LocalBuffer)
			stbi_image_free(m_LocalBuffer);
	}

	OpenGLTexture2D::OpenGLTexture2D(const uint32_t width, const uint32_t height):
		m_Type(""),
		m_Width(width),
		m_Height(height),
		m_DataFormat(GL_RGBA),
		m_InternalFormat(GL_RGBA8)
	{
		HZ_PROFILE_FUNCTION();

		glCreateTextures(GL_TEXTURE_2D, 1, &this->m_RendererID);

		glTextureStorage2D(this->m_RendererID, 1, m_InternalFormat, this->m_Width, this->m_Height);

		glTextureParameteri(this->m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(this->m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(this->m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(this->m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture2D::OpenGLTexture2D(std::string filepath, std::string type="") :
		m_Type(type), m_Path(filepath),
		m_InternalFormat(0),m_DataFormat(0)
	{
		HZ_PROFILE_FUNCTION();

		int width, height, channels;
		//上下翻转,opengl以左下角为（0， 0）点，相当于从下往上
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* m_LocalBuffer = stbi_load(filepath.c_str(), &width, &height, &channels, 0);

		HZ_ASSERT(m_LocalBuffer, "加载纹理失败");

		this->m_Width = width;
		this->m_Height = height;

		switch (channels)
		{
		case 3:
			this->m_InternalFormat = GL_RGB8;
			this->m_DataFormat = GL_RGB;
			break;
		case 4:
			this->m_InternalFormat = GL_RGBA8;
			this->m_DataFormat = GL_RGBA;
			break;
		default:
			HZ_CORE_ASSERT(0, "Dont't support this picture type");
			break;
		}

		glCreateTextures(GL_TEXTURE_2D, 1, &this->m_RendererID);
		glTextureStorage2D(this->m_RendererID, 1, this->m_InternalFormat, this->m_Width, this->m_Height);

		glTextureParameteri(this->m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(this->m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTextureParameteri(this->m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTextureParameteri(this->m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureSubImage2D(this->m_RendererID, 0, 0, 0, this->m_Width, this->m_Height, this->m_DataFormat, GL_UNSIGNED_BYTE, m_LocalBuffer);
		
		if (m_LocalBuffer)
			stbi_image_free(m_LocalBuffer);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &this->m_RendererID);
	}

	bool OpenGLTexture2D::operator == (const Texture& other)
	{
		return this->m_RendererID == ((OpenGLTexture2D&)other).m_RendererID;
	}
	
	void OpenGLTexture2D::Bind(uint32_t slot)
	{
		//glActiveTexture(GL_TEXTURE0 + slot);
		//glBindTexture(GL_TEXTURE_2D, this->m_RendererID);
		glBindTextureUnit(slot, this->m_RendererID);
	}
	
	void OpenGLTexture2D::UnBind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		HZ_PROFILE_FUNCTION();

		uint32_t bbp = this->m_DataFormat == GL_RGBA ? 4 : 3;
		HZ_CORE_ASSERT(size == this->m_Width * this->m_Height * bbp, "Don't must be entire texture!");
		glTextureSubImage2D(this->m_RendererID, 0, 0, 0, this->m_Width, this->m_Height, this->m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::ActivateTexture(uint32_t slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
	}
}

