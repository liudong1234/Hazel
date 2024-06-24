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

		//暂变为public
		std::vector<OpenGLMesh> meshes;
		std::vector<OpenGLTexture2D> texturesLoaded;
	private:
		//模型数据
		std::string directory;
	
		//最终纹理
		std::vector<Ref<OpenGLTexture2D>> textureResult;
		//函数
		//模型导入
		void LoadModel(std::string path);
		//数据导入
		void ProcessNode(aiNode* node, const aiScene* scene);
		//将Assimp的数据解析到Mesh类中
		OpenGLMesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	
		void LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	};


}
