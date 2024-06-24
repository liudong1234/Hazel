#pragma once

#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/Buffer.h"
#include "Hazel/Renderer/VertexArray.h"
#include "Hazel/Renderer/Texture.h"

namespace Hazel
{
#define MAX_BONE_INFLUENCE 4
	struct Vertex {
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
		glm::vec3 Tangent;
		glm::vec3 Bitangent;
		int m_BoneIDs[MAX_BONE_INFLUENCE];
		//weights from each bone
		float m_Weights[MAX_BONE_INFLUENCE];
	};

	class Mesh
	{
	public:
		static Ref<Mesh> Create(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Ref<Texture2D>> vTexture);
	};
}