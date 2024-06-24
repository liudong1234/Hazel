#include "hzpch.h"
#include "Renderer3D.h"
#include "Model.h"

namespace Hazel
{
	Renderer3D::SceneData* Renderer3D::m_SceneData = new Renderer3D::SceneData;

	void Renderer3D::Init(std::string path)
	{
		m_SceneData->directory = path;
		LoadModel(m_SceneData->directory);

		ShaderLibray shaderLibrary;
		m_SceneData->shader = shaderLibrary.Load("Assets/shaders/Basic.glsl");
	}

	void Renderer3D::BeginScene(const FPSCamera& camera)
	{
		m_SceneData->shader->Bind();
		m_SceneData->shader->SetUniformMat4("projection", camera.GetProjectionMatrix());
		m_SceneData->shader->SetUniformMat4("view", camera.GetViewMatrix());

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
		m_SceneData->shader->SetUniformMat4("model", transform);

		m_SceneData->shader->SetUniformFloat3("cameraPos", camera.GetPosition());
		m_SceneData->shader->SetUniformFloat3("light.position", { 1.2f, 1.0f, 2.0f });
		m_SceneData->shader->SetUniformFloat3("light.ambient", { 0.6f, 0.6f, 0.6f });
		m_SceneData->shader->SetUniformFloat3("light.diffuse", { 0.8f, 0.7f, 0.6f });
		m_SceneData->shader->SetUniformFloat3("light.specular", {0.1f, 0.1f, 0.1f });
		m_SceneData->shader->SetUniformFloat1("material.shininess", 32.0f);

		ShowModel();
	}

	void Renderer3D::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewPort(0, 0, width, height);
	}

	void Renderer3D::EndScend()
	{

	}

	void Renderer3D::Shutdown()
	{
		delete m_SceneData;
	}

	void Renderer3D::ShowModel()
	{
		for (unsigned int i = 0; i < m_SceneData->meshes.size(); i++)
			m_SceneData->meshes[i]->Draw(m_SceneData->shader);
	}

	void Renderer3D::LoadModel(std::string path)
	{
		Assimp::Importer importer;
		/*通过设定aiProcess_Triangulate，我们告诉Assimp，如果模型不是（全部）由三角形组成，它需要将模型所有的图元形状变换为三角形。aiProcess_FlipUVs将在处理的时候翻转y轴的纹理坐标*/
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "ERROR:ASSIMP::" << importer.GetErrorString() << std::endl;
			return;
		}
		m_SceneData->directory = path.substr(0, path.find_last_of('/'));
		ProcessNode(scene->mRootNode, scene);
	}

	void Renderer3D::ProcessNode(aiNode* node, const aiScene* scene)
	{
		//处理节点所有的网格（如果有的话）
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			m_SceneData->meshes.push_back(ProcessMesh(mesh, scene));
		}
		//对它的子节点重复这一过程
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene);
		}
	}
	
	Ref<OpenGLMesh> Renderer3D::ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			//处理顶点位置，法线，和纹理坐标
			glm::vec3 position;//位置
			position.x = mesh->mVertices[i].x;
			position.y = mesh->mVertices[i].y;
			position.z = mesh->mVertices[i].z;
			vertex.Position = position;

			if (mesh->HasNormals())
			{
				glm::vec3 normal; //法线
				normal.x = mesh->mNormals[i].x;
				normal.y = mesh->mNormals[i].y;
				normal.z = mesh->mNormals[i].z;
				vertex.Normal = normal;
			}

			if (mesh->mTextureCoords[0])//网格中是否有纹理坐标
			{
				glm::vec2 texcoord;//纹理
				texcoord.x = mesh->mTextureCoords[0][i].x;
				texcoord.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = texcoord;
				////tangent
				if (mesh->HasTangentsAndBitangents())
				{
					glm::vec3 tangent;
					tangent.x = mesh->mTangents[i].x;
					tangent.y = mesh->mTangents[i].y;
					tangent.z = mesh->mTangents[i].z;
					vertex.Tangent = tangent;
					//// bitangent
					glm::vec3 biTangent;
					biTangent.x = mesh->mBitangents[i].x;
					biTangent.y = mesh->mBitangents[i].y;
					biTangent.z = mesh->mBitangents[i].z;
					vertex.Bitangent = biTangent;
				}
			}
			else
			{
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);
			}
			vertices.push_back(vertex);
		}

		//处理索引
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		//处理材质
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
			LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
			LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
		}
		Ref<OpenGLMesh> smesh = std::make_shared<OpenGLMesh>(std::move(vertices), std::move(indices), std::move(m_SceneData->textureResult));

		return smesh;
	}

	void Renderer3D::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
	{
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			bool skip = false;
			mat->GetTexture(type, i, &str);
			
			std::string path = m_SceneData->directory + '/' + str.C_Str();

			for (unsigned int j = 0; j < m_SceneData->texturesLoaded.size(); j++)
			{
				if (std::strcmp(m_SceneData->texturesLoaded[j]->GetPath().c_str(), path.c_str()) == 0)
				{
					m_SceneData->textureResult.push_back(m_SceneData->texturesLoaded[j]);
					skip = true;
					break;
				}
			}
			if (!skip)
			{
				// Create a new unique_ptr for each texture
				std::string filename = std::string(str.C_Str());
				filename = m_SceneData->directory + '/' + filename;

				Ref<OpenGLTexture2D> texture = std::make_shared<OpenGLTexture2D>(filename, typeName);
				m_SceneData->textureResult.push_back(texture);
				m_SceneData->texturesLoaded.push_back(texture);
			}
		}
	}

}

