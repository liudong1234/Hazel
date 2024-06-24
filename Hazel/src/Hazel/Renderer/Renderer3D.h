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
		static void BeginScene(const FPSCamera& camera); //������ʼ
		static void OnWindowResize(uint32_t width, uint32_t height);

		static void EndScend(); //

		static void Shutdown();

		static void ShowModel();

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData
		{
			//ģ������
			std::string directory;

			std::vector<Ref<OpenGLMesh>> meshes;
			std::vector<Ref<OpenGLTexture2D>> texturesLoaded;
			//��������
			std::vector<Ref<OpenGLTexture2D>> textureResult;
			Ref<Shader> shader;
		};

		static SceneData* m_SceneData;
	private:
		//ģ�͵���
		static void LoadModel(std::string path);
		//���ݵ���
		static void ProcessNode(aiNode* node, const aiScene* scene);
		//��Assimp�����ݽ�����Mesh����
		static Ref<OpenGLMesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);

		static void LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);


	};

}