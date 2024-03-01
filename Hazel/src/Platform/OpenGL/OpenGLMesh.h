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
		std::vector<Ref<OpenGLTexture2D>> textures;
		//函数
		OpenGLMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Ref<OpenGLTexture2D>> vTexture);
		~OpenGLMesh();
		void Draw(std::shared_ptr<Shader>& shader);

	private:
		std::shared_ptr<OpenGLVertexArray> m_VertexArray;
		void SetOpenGLMesh();

	public:
	};

}