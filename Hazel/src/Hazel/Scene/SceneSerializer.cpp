#include "hzpch.h"
#include "SceneSerializer.h"

#include "Entity.h"
#include "Components.h"

#include <yaml-cpp/yaml.h>

namespace YAML
{
	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}
		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}
		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};
	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}
		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

}


namespace Hazel
{
	YAML::Emitter& operator << (YAML::Emitter& out, const glm::vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}
	YAML::Emitter& operator << (YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}
	YAML::Emitter& operator << (YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	static std::string RigidBody2DBodyTypeToSTring(RigidBody2DComponent::BodyType bodyType)
	{
		switch (bodyType)
		{
			case RigidBody2DComponent::BodyType::Static:
				return "Static";
				break;
			case RigidBody2DComponent::BodyType::Dynamic:
				return "Dynamic";
				break;
			case RigidBody2DComponent::BodyType::Kinematic:
				return "Kinematic";
				break;
			default:
				break;
		}
		HZ_CORE_ASSERT(false, "Unknown body type");
		return "";

	}

	static RigidBody2DComponent::BodyType RigidBody2DBodyTypeFromString(const std::string& type)
	{
		if (type == "Dynamic")
			return RigidBody2DComponent::BodyType::Dynamic;
		else if (type == "Kinematic")
			return RigidBody2DComponent::BodyType::Kinematic;
		else
			return RigidBody2DComponent::BodyType::Static;		
	}


    SceneSerializer::SceneSerializer(const Ref<Scene>& scene):
        m_Scene(scene)
    {
    }

	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		HZ_CORE_ASSERT(entity.HasComponent<IDComponent>(), "ERROR UUID");

		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << entity.GetUUID();
		if (entity.HasComponent<TagComponent>())
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap;
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			out << YAML::Key << "Tag" << YAML::Value << tag;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap;

			auto& tc = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "Translation" << YAML::Value << tc.Translation;
			out << YAML::Key << "Scale" << YAML::Value << tc.Scale;
			out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap;

			auto& cameraComponent = entity.GetComponent<CameraComponent>();
			auto& camera = cameraComponent.Camera;
			out << YAML::Key << "Primary" << YAML::Value << cameraComponent.Primary;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.FixedAspectRatio;
			out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
			out << YAML::Key << "PerspectiveFov" << YAML::Value << camera.GetPerspectiveVerticalFOV();
			out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspectiveNear();
			out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspectiveFar();
			out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
			out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicNear();
			out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicFar();
			out << YAML::EndMap;
		}
		if (entity.HasComponent<SpriteRendererComponent>())
		{
			out << YAML::Key << "SpriteComponent";
			out << YAML::BeginMap;
			auto& sprite = entity.GetComponent<SpriteRendererComponent>();
			out << YAML::Key << "Color" << YAML::Value << sprite.Color;
			out << YAML::EndMap;
		}
		if (entity.HasComponent<CircleRendererComponent>())
		{
			out << YAML::Key << "CircleRendererComponent";
			out << YAML::BeginMap;
			auto& circle = entity.GetComponent<CircleRendererComponent>();
			out << YAML::Key << "Color" << YAML::Value << circle.Color;
			out << YAML::Key << "Thickness" << YAML::Value << circle.Thickness;
			out << YAML::Key << "Fade" << YAML::Value << circle.Fade;
			out << YAML::EndMap;
		}
		if (entity.HasComponent<RigidBody2DComponent>())
		{
			out << YAML::Key << "RigidBody2DComponent";
			out << YAML::BeginMap;
			auto& rb2d = entity.GetComponent<RigidBody2DComponent>();
			out << YAML::Key << "RigidBodyType" << YAML::Value << RigidBody2DBodyTypeToSTring(rb2d.Type);
			out << YAML::Key << "FixedRotation" << YAML::Value << rb2d.FixedRotation;
			out << YAML::EndMap;
		}
		if (entity.HasComponent<BoxCollider2DComponent>())
		{
			out << YAML::Key << "BoxCollider2DComponent";
			out << YAML::BeginMap;
			auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();
			out << YAML::Key << "Offset" << YAML::Value << bc2d.Offset;
			out << YAML::Key << "Size" << YAML::Value << bc2d.Size;
			out << YAML::Key << "Density" << YAML::Value << bc2d.Density;
			out << YAML::Key << "Friction" << YAML::Value << bc2d.Friction;
			out << YAML::Key << "Restitution" << YAML::Value << bc2d.Restitution;
			out << YAML::Key << "Restitution Threshold" << YAML::Value << bc2d.RestitutionThreshold;
			out << YAML::EndMap;
		}
		if (entity.HasComponent<CircleCollider2DComponent>())
		{
			out << YAML::Key << "CircleCollider2DComponent";
			out << YAML::BeginMap;
			auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();
			out << YAML::Key << "Offset" << YAML::Value << cc2d.Offset;
			out << YAML::Key << "Radius" << YAML::Value << cc2d.Radius;
			out << YAML::Key << "Density" << YAML::Value << cc2d.Density;
			out << YAML::Key << "Friction" << YAML::Value << cc2d.Friction;
			out << YAML::Key << "Restitution" << YAML::Value << cc2d.Restitution;
			out << YAML::Key << "Restitution Threshold" << YAML::Value << cc2d.RestitutionThreshold;
			out << YAML::EndMap;
		}
		out << YAML::EndMap;
	}

    void SceneSerializer::Serialize(const std::string& filepath)
    {
        YAML::Emitter out;
        out << YAML::BeginMap;
        out << YAML::Key << "Scene";
        out << YAML::Value << "Name";
        out << YAML::Key << "Entities";
        out << YAML::Value << YAML::BeginSeq;
        for (auto en : this->m_Scene->m_Registry.view<entt::entity>())
        {
            Entity entity(en, this->m_Scene.get());
            if (!entity)
                return;
			SerializeEntity(out, entity);
        }
		out << YAML::EndSeq;
		out << YAML::EndMap;
		std::ofstream fout(filepath);
		fout << out.c_str();
    }

    void SceneSerializer::SerializeRuntime(const std::string& filepath)
    {
        HZ_CORE_ASSERT(false, "");
    }
    
    bool SceneSerializer::Deserialize(const std::string& filepath)
    {
		//bool fl = filepath.find_last_not_of(".hazel");
		if (filepath.find(".hazel") == -1)
		{
			HZ_CORE_ERROR("Failed to load this file '{0}'\n", filepath);
			return false;
		}
		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();
		HZ_CORE_TRACE("DeSerialize Scene {0}", sceneName);

		YAML::Node entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				uint64_t uuid = entity["Entity"].as<uint64_t>();

				std::string name;
				auto& tagComponent = entity["TagComponent"];
				if (tagComponent)
					name = tagComponent["Tag"].as<std::string>();

				HZ_CORE_TRACE("DeSerialize entity with ID = {0}, name = {1}", uuid, name);

				Entity& deserialedEntity = this->m_Scene->CreateEntityWithUUID(uuid, name);

				auto& transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					auto& tc = deserialedEntity.GetComponent<TransformComponent>();
					tc.Translation = transformComponent["Translation"].as<glm::vec3>();
					tc.Scale = transformComponent["Scale"].as<glm::vec3>();
					tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
				}

				auto& cameraComponent = entity["CameraComponent"];
				if (cameraComponent)
				{
					auto& cc = deserialedEntity.AddComponent<CameraComponent>();
					auto& camera = cc.Camera;

					cc.Primary = cameraComponent["Primary"].as<bool>();
					cc.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();

					//auto& cameraPos = cameraComponent["Camera"];
					camera.SetProjectionType((SceneCamera::ProjectionType)cameraComponent["ProjectionType"].as<int>());
					camera.SetPerspectiveVerticalFOV(cameraComponent["PerspectiveFov"].as<float>());
					camera.SetPerspectiveFar(cameraComponent["PerspectiveFar"].as<float>());
					camera.SetPerspectiveNear(cameraComponent["PerspectiveNear"].as<float>());
					camera.SetOrthographicSize(cameraComponent["OrthographicSize"].as<float>());
					camera.SetOrthographicFar(cameraComponent["OrthographicFar"].as<float>());
					camera.SetOrthographicNear(cameraComponent["OrthographicNear"].as<float>());
				}

				auto& spriteComponent = entity["SpriteComponent"];
				if (spriteComponent)
				{
					auto& sprite = deserialedEntity.AddComponent<SpriteRendererComponent>();
					sprite.Color = spriteComponent["Color"].as<glm::vec4>();
				}
				auto& circleComponent = entity["CircleRendererComponent"];
				if (circleComponent)
				{
					auto& circle = deserialedEntity.AddComponent<CircleRendererComponent>();
					circle.Color = circleComponent["Color"].as<glm::vec4>();
					circle.Thickness = circleComponent["Thickness"].as<float>();
					circle.Fade = circleComponent["Fade"].as<float>();
				}

				auto& rb2dComponent = entity["RigidBody2DComponent"];
				if (rb2dComponent)
				{
					auto& rigidBody = deserialedEntity.AddComponent<RigidBody2DComponent>();
					rigidBody.Type = RigidBody2DBodyTypeFromString(rb2dComponent["RigidBodyType"].as<std::string>());
					rigidBody.FixedRotation = rb2dComponent["FixedRotation"].as<bool>();
				}

				auto& bc2dComponent = entity["BoxCollider2DComponent"];
				if (bc2dComponent)
				{
					auto& boxcollider = deserialedEntity.AddComponent<BoxCollider2DComponent>();
					boxcollider.Offset = bc2dComponent["Offset"].as<glm::vec2>();
					boxcollider.Size = bc2dComponent["Size"].as<glm::vec2>();
					boxcollider.Density = bc2dComponent["Density"].as<float>();
					boxcollider.Friction = bc2dComponent["Friction"].as<float>();
					boxcollider.Restitution = bc2dComponent["Restitution"].as<float>();
					boxcollider.RestitutionThreshold = bc2dComponent["Restitution Threshold"].as<float>();
				}
				
				auto& cc2dComponent = entity["CircleCollider2DComponent"];
				if (cc2dComponent)
				{
					auto& circlecollider = deserialedEntity.AddComponent<CircleCollider2DComponent>();
					circlecollider.Offset = cc2dComponent["Offset"].as<glm::vec2>();
					circlecollider.Radius = cc2dComponent["Radius"].as<float>();
					circlecollider.Density = cc2dComponent["Density"].as<float>();
					circlecollider.Friction = cc2dComponent["Friction"].as<float>();
					circlecollider.Restitution = cc2dComponent["Restitution"].as<float>();
					circlecollider.RestitutionThreshold = cc2dComponent["Restitution Threshold"].as<float>();
				}

			}
		}

        return true;
    }
    
    bool SceneSerializer::DeserializeRuntime(const std::string& filepath)
    {
        return false;
    }
}
