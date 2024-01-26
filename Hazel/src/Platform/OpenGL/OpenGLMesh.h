#pragma once

#include "OpenGLShader.h"
#include "OpenGLVertexArray.h"
#include "OpenGLBuffer.h"
#include "OpenGLTexture.h"
#include "Hazel/Renderer/Mesh.h"
#include <memory>

namespace Hazel
{
	class OpenGLMesh:
		public Mesh
	{
	public:
		//网格数据
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<OpenGLTexture2D> textures;
		//函数
		OpenGLMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<OpenGLTexture2D> vTexture);
		~OpenGLMesh();
		void Draw(std::shared_ptr<Shader>& shader);

	private:
		std::shared_ptr<OpenGLVertexArray> m_VertexArray;
		std::shared_ptr<OpenGLIndexBuffer> m_IndexBuffer;
		std::shared_ptr<OpenGLVertexBuffer> m_VertexBuffer;
		std::shared_ptr<OpenGLVertexArray> m_LightVAO;

		void SetOpenGLMesh();

	public:
	};

}