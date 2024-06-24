#pragma once
#include "OpenGLMesh.h"
#include "Hazel/Renderer/Model.h"

#include "Assimp/defs.h"
#include "Assimp/Importer.hpp"
#include "assimp/scene.h"
#include "Assimp/postprocess.h"

namespace Hazel
{
	class OpenGLModel:
		public Model
	{
	public:
		OpenGLModel() {}
		OpenGLModel(const char* path);
		~OpenGLModel();
		virtual void Render(std::shared_ptr<Shader>& shader) override;

		//�ݱ�Ϊpublic
		std::vector<OpenGLMesh> meshes;
		std::vector<OpenGLTexture2D> texturesLoaded;
	private:
		//ģ������
		std::string directory;
	
		//��������
		std::vector<Ref<OpenGLTexture2D>> textureResult;
		//����
		//ģ�͵���
		void LoadModel(std::string path);
		//���ݵ���
		void ProcessNode(aiNode* node, const aiScene* scene);
		//��Assimp�����ݽ�����Mesh����
		OpenGLMesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	
		void LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	};


}
