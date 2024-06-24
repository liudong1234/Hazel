#include "hzpch.h"
#include "OpenGLModel.h"
#include "glad/glad.h"
#include "stb_image.h"

namespace Hazel
{
	unsigned int TextureFromFile(const char* path, const std::string& directory, [[maybe_unused]] bool gamma);

	OpenGLModel::OpenGLModel(const char* path)
	{
		this->LoadModel(path);
	}

	OpenGLModel::~OpenGLModel()
	{
		//std::cout << "OpenGLModel 析构" << std::endl;
	}

	void OpenGLModel::Render(std::shared_ptr<Shader>& shader)
	{
		for (unsigned int i = 0; i < this->meshes.size(); i++)
			this->meshes[i].Draw(shader);
	}

	//加载
	void OpenGLModel::LoadModel(std::string path)
	{
		Assimp::Importer importer;
		/*通过设定aiProcess_Triangulate，我们告诉Assimp，如果模型不是（全部）由三角形组成，它需要将模型所有的图元形状变换为三角形。aiProcess_FlipUVs将在处理的时候翻转y轴的纹理坐标*/
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "ERROR:ASSIMP::" << importer.GetErrorString() << std::endl;
			return;
		}
		this->directory = path.substr(0, path.find_last_of('/'));
		this->ProcessNode(scene->mRootNode, scene);
	}

	//处理节点
	void OpenGLModel::ProcessNode(aiNode* node, const aiScene* scene)
	{
		//处理节点所有的网格（如果有的话）
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			this->meshes.push_back(ProcessMesh(mesh, scene));
		}
		//对它的子节点重复这一过程
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene);
		}
	}

	//处理网格
	OpenGLMesh OpenGLModel::ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<OpenGLTexture2D> textures;

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
					->textureResult->push_back(texturesLoaded[j]);
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
				unsigned int id = TextureFromFile(str.C_Str(), this->directory, true);
				texture.SetId(id);
				this->textureResult.push_back(texture);
				this->texturesLoaded.push_back(texture);
			}
		}
	}

	unsigned int TextureFromFile(const char* path, const std::string& directory, [[maybe_unused]] bool gamma)
	{
		std::string filename = std::string(path);
		filename = directory + '/' + filename;

		unsigned int textureID;
		glGenTextures(1, &textureID);

		int width, height, nrComponents;
		unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
			GLenum internalformat = 0;
			GLenum dataformat = 0;
			if (nrComponents == 1)
			{
				internalformat = dataformat = GL_RED;
			}
			else if (nrComponents == 3)
			{
				internalformat = gamma ? GL_SRGB : GL_RGB;
				dataformat = GL_RGB;
			}
			else if (nrComponents == 4)
			{
				internalformat = gamma ? GL_SRGB : GL_RGB;
				dataformat = GL_RGBA;
			}

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, dataformat, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
		}
		else
		{
			std::cout << "Texture failed to load at path: " << path << std::endl;
			stbi_image_free(data);
		}

		return textureID;
	}
}