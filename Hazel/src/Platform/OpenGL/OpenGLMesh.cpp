#include "hzpch.h"
#include "OpenGLMesh.h"
#include "Hazel/Renderer/Renderer.h"
namespace Hazel
{
	OpenGLMesh::OpenGLMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<OpenGLTexture2D> vTexture):
		vertices(vertices),
		indices(indices),
		textures(vTexture)
	{
		this->m_VertexArray = std::make_shared<OpenGLVertexArray>();
		this->m_IndexBuffer = std::make_shared<OpenGLIndexBuffer>(&indices[0], indices.size());

		this->m_VertexBuffer = std::make_shared<OpenGLVertexBuffer>(&vertices[0], vertices.size() * sizeof(Vertex));


		BufferLayout layout =
		{
			{ "g_Position", Hazel::ShaderDataType::Float3 },
			{ "g_Normal", Hazel::ShaderDataType::Float4 },
			{"g_TexCoord", ShaderDataType::Float2},
			{"g_Tangent", ShaderDataType::Float3},
			{"g_Bitangent", ShaderDataType::Float3}
		};
		this->m_VertexBuffer->SetLayout(layout);
		this->m_VertexArray->AddVertexBuffer(this->m_VertexBuffer);
		this->m_VertexArray->SetIndexBuffer(this->m_IndexBuffer);
	}

	OpenGLMesh::~OpenGLMesh()
	{
		
	}

	void OpenGLMesh::Draw(std::shared_ptr<Shader>& shader)
	{
		//可能需要修改
		shader->Bind();
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;
		for (unsigned int i = 0; i < this->textures.size(); i++)
		{
			OpenGLTexture2D::ActivateTexture(i); // 在绑定之前激活相应的纹理单元
			std::string number;
			std::string name = this->textures[i].GetType();
			if (name == "texture_diffuse")
				number = std::to_string(diffuseNr++);
			else if (name == "texture_specular")
				number = std::to_string(specularNr++);
			else if (name == "texture_normal")
				number = std::to_string(normalNr++);
			else if (name == "texture_height")
				number = std::to_string(heightNr++);

			std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformInt(("material." + name + number).c_str(), i);

			this->textures[i].Bind();
		}

		//绘制网格
		Renderer::Submit(this->m_VertexArray, shader);
	}

	void OpenGLMesh::SetOpenGLMesh()
	{

	}

}

