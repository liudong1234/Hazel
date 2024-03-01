#pragma once

#include "Hazel/Renderer/Texture.h"



namespace Hazel
{
	typedef unsigned int GLenum;
	class OpenGLTexture2D :
		public Texture2D
	{
	public:
		OpenGLTexture2D(std::string filepath);
		OpenGLTexture2D(const uint32_t width, const uint32_t height);
		OpenGLTexture2D(std::string filepath, std::string type);
		virtual ~OpenGLTexture2D();

		virtual bool operator == (const Texture& other) override;

		virtual void Bind(uint32_t slot = 0) override;
		virtual void UnBind() override;

		virtual void SetData(void* data, uint32_t size) override;

		static void ActivateTexture(uint32_t slot = 0);
		inline std::string GetType() { return this->m_Type; }
		inline std::string GetPath() { return this->m_Path; }
		inline void SetId(uint32_t id) { this->m_RendererID = id; }
		inline uint32_t GetId() { return this->m_RendererID; }
	private:
		uint32_t m_RendererID;
		//unsigned char* m_LocalBuffer;
		uint32_t m_Width, m_Height;
		std::string m_Type;
		std::string m_Path;
		
		GLenum m_DataFormat;
		GLenum m_InternalFormat;
	};
}