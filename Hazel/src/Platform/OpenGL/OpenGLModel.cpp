#include "hzpch.h"
#include "OpenGLModel.h"

namespace Hazel
{
	OpenGLModel::OpenGLModel(const char* path)
	{
		this->LoadModel(path);
	}

	OpenGLModel::~OpenGLModel()
	{
		//std::cout << "OpenGLModel ����" << std::endl;
	}

	void OpenGLModel::Draw(std::shared_ptr<Shader>& shader)
	{
		for (unsigned int i = 0; i < this->meshes.size(); i++)
			this->meshes[i].Draw(shader);
	}

	//����
	void OpenGLModel::LoadModel(std::string path)
	{
		Assimp::Importer importer;
		/*ͨ���趨aiProcess_Triangulate�����Ǹ���Assimp�����ģ�Ͳ��ǣ�ȫ��������������ɣ�����Ҫ��ģ�����е�ͼԪ��״�任Ϊ�����Ρ�aiProcess_FlipUVs���ڴ����ʱ��תy�����������*/
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "ERROR:ASSIMP::" << importer.GetErrorString() << std::endl;
			return;
		}
		this->directory = path.substr(0, path.find_last_of('/'));
		this->ProcessNode(scene->mRootNode, scene);
	}

	//����ڵ�
	void OpenGLModel::ProcessNode(aiNode* node, const aiScene* scene)
	{
		//����ڵ����е���������еĻ���
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			this->meshes.push_back(ProcessMesh(mesh, scene));
		}
		//�������ӽڵ��ظ���һ����
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene);
		}
	}

	//��������
	OpenGLMesh OpenGLModel::ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<OpenGLTexture2D> textures;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			//������λ�ã����ߣ�����������
			glm::vec3 position;//λ��
			position.x = mesh->mVertices[i].x;
			position.y = mesh->mVertices[i].y;
			position.z = mesh->mVertices[i].z;
			vertex.Position = position;

			if (mesh->HasNormals())
			{
				glm::vec3 normal; //����
				normal.x = mesh->mNormals[i].x;
				normal.y = mesh->mNormals[i].y;
				normal.z = mesh->mNormals[i].z;
				vertex.Normal = normal;
			}

			if (mesh->mTextureCoords[0])//�������Ƿ�����������
			{
				glm::vec2 texcoord;//����
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
		//��������
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		//�������
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			this->LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			this->LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
			this->LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
			this->LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
		}

		OpenGLMesh meshObject(vertices, indices, this->textureResult);

		return meshObject;

	}

	void OpenGLModel::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
	{
		std::vector<OpenGLTexture2D> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			bool skip = false;
			mat->GetTexture(type, i, &str);
			for (unsigned int j = 0; j < texturesLoaded.size(); j++)
			{
				if (std::strcmp(texturesLoaded[j].GetPath().c_str(), str.C_Str()) == 0)
				{
					this->textureResult.push_back(texturesLoaded[j]);
					skip = true;
					break;
				}
			}
			if (!skip)
			{
				// Create a new unique_ptr for each texture
				std::string filename = std::string(str.C_Str());
				filename = directory + '/' + filename;
				OpenGLTexture2D texture(filename, typeName);
				//texture.id = TextureFromFile(str.C_Str(), this->directory, true);
				//texture.type = typeName;
				//texture.path = str.C_Str();
				this->textureResult.push_back(texture);
				this->texturesLoaded.push_back(texture);
			}
		}
	}
}
