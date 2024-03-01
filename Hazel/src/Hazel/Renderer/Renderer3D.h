#pragma once

#include "RenderCommand.h"
#include "FPSCamera.h"
#include "Shader.h"
#include "Mesh.h"

#include "Platform/OpenGL/OpenGLTexture.h"
#include "Platform/OpenGL/OpenGLMesh.h"

#include "Assimp/defs.h"
#include "Assimp/Importer.hpp"
#include "assimp/scene.h"
#include "Assimp/postprocess.h"


namespace Hazel
{
	class Renderer3D
	{
	public:
		static void Init(std::string path);
		static void BeginScene(const FPSCamera& camera); //场景开始
		static void OnWindowResize(uint32_t width, uint32_t height);

		static void EndScend(); //

		static void Shutdown();

		static void ShowModel();

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData
		{
			//模型数据
			std::string directory;

			std::vector<Ref<OpenGLMesh>> meshes;
			std::vector<Ref<OpenGLTexture2D>> texturesLoaded;
			//最终纹理
			std::vector<Ref<OpenGLTexture2D>> textureResult;
			Ref<Shader> shader;
		};

		static SceneData* m_SceneData;
	private:
		//模型导入
		static void LoadModel(std::string path);
		//数据导入
		static void ProcessNode(aiNode* node, const aiScene* scene);
		//将Assimp的数据解析到Mesh类中
		static Ref<OpenGLMesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);

		static void LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);


	};

}