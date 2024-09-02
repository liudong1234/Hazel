#pragma once

#include "SceneCamera.h"
//#include "ScriptableEntity.h"
#include "Hazel/Renderer/Texture.h"
#include "Hazel/Core/UUID.h"


#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Hazel
{
	struct IDComponent
	{
		UUID ID;
		IDComponent() = default;
		IDComponent(const IDComponent&) = default;
	};

    struct TagComponent
    {
        std::string Tag;

        TagComponent() = default;
        TagComponent(const TagComponent&) = default;
        TagComponent(const std::string& tag) :
            Tag(tag) {}

        operator std::string& () { return this->Tag; }//隐式强制类型转换
        operator const std::string& () const { return this->Tag; }
    };


    struct TransformComponent
    {
        glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
        glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
        glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::vec3& translation) :
            Translation(translation) {}
        
        glm::mat4 GetTransform() const
        {
            /*glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f)) *
                glm::rotate(glm::mat4(1.0f), Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f)) *
                glm::rotate(glm::mat4(1.0f), Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
				*/
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));
            return glm::translate(glm::mat4(1.0f), Translation) * rotation *
                glm::scale(glm::mat4(1.0f), Scale);
        }

        //operator glm::mat4& () { return this->Transform; }//隐式强制类型转换
        //operator const glm::mat4& () const { return this->Transform; }
    };

    struct SpriteRendererComponent
    {
        glm::vec4 Color{ 1.0f , 1.0f, 1.0f, 1.0f };
		Ref<Texture2D> Texture;
		float TillingFactor = 1.0f;
        SpriteRendererComponent() = default;
        SpriteRendererComponent(const SpriteRendererComponent&) = default;
        SpriteRendererComponent(const glm::vec4& color) :
            Color(color) {}
    };

	struct CircleRendererComponent
	{
		glm::vec4 Color{ 1.0f , 1.0f, 1.0f, 1.0f };
		//float Radis = 0.5f;
		float Thickness = 1.0f;
		float Fade = 0.005f;//模糊程度
		CircleRendererComponent() = default;
		CircleRendererComponent(const CircleRendererComponent&) = default;
	};

    struct CameraComponent
    {
        SceneCamera Camera;
        bool Primary = true;//是否为主相机
        bool FixedAspectRatio = false;//是否保持纵横比

        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;

    };

	class ScriptableEntity;
    struct NativeScriptComponent
    {
        ScriptableEntity* Instance = nullptr;
        /*
        std::function<void()> InstantiateFunction;//实例化
        std::function<void()> DestroyInstantiateFunction; //销毁

        std::function<void(ScriptableEntity*)> OnCreateFunction; // 创建函数
        std::function<void(ScriptableEntity*)> OnDestroyFunction; // 销毁函数
        std::function<void(ScriptableEntity*, TimeStep)> OnUpdateFunction; // 更新函数
        */
        ScriptableEntity*(*InstantiateScript)();
        void (*DestroyScript)(NativeScriptComponent*);

        template<typename T>
        void Bind()
        {
            InstantiateScript = []() {return static_cast<ScriptableEntity*>(new T()); };
            DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
            /*
            InstantiateFunction = [&] () { Instance = new T(); };
            DestroyInstantiateFunction = [&]() { delete (T*)Instance;  Instance = nullptr; };

            OnCreateFunction = [](ScriptableEntity* instance) { ((T*)instance)->OnCreate(); };
            OnDestroyFunction = [](ScriptableEntity* instance) { ((T*)instance)->OnDestroy(); };
            OnUpdateFunction = [](ScriptableEntity* instance, TimeStep ts) { ((T*)instance)->OnUpdate(ts); };
            */
        }
    };

	//刚体
	struct RigidBody2DComponent
	{
		enum class BodyType
		{
			Static = 0,
			Dynamic,
			Kinematic,
		};
		bool FixedRotation = false;
		BodyType Type = BodyType::Static;

		//Storage for runtime
		void* RuntimeBody = nullptr;

		RigidBody2DComponent() = default;
		RigidBody2DComponent(const RigidBody2DComponent&) = default;

	};

	//碰撞体
	struct BoxCollider2DComponent
	{

		glm::vec2 Offset = { 0.0f, 0.0f };
		glm::vec2 Size = { 0.5f, 0.5f };

		//TODO Material in future
		float Density = 1.0f;//密度
		float Friction = 0.5f;//摩擦
		float Restitution = 0.0f;//弹性？
		float RestitutionThreshold = 0.5f;//阈值

		//Storage for runtime
		void* RuntimeFixture = nullptr;

		BoxCollider2DComponent() = default;
		BoxCollider2DComponent(const BoxCollider2DComponent&) = default;

	};

	struct CircleCollider2DComponent
	{

		glm::vec2 Offset = { 0.0f, 0.0f };
		float Radius = 0.5f;

		//TODO Material in future
		float Density = 1.0f;//密度
		float Friction = 0.5f;//摩擦
		float Restitution = 0.0f;//弹性？
		float RestitutionThreshold = 0.5f;//阈值

		//Storage for runtime
		void* RuntimeFixture = nullptr;

		CircleCollider2DComponent() = default;
		CircleCollider2DComponent(const CircleCollider2DComponent&) = default;

	};

	template<typename... Component>
	struct ComponentGroup
	{

	};
	using AllComponents =
		ComponentGroup<TransformComponent, SpriteRendererComponent,
		CircleRendererComponent, CameraComponent, NativeScriptComponent,
		RigidBody2DComponent, BoxCollider2DComponent, CircleCollider2DComponent>;

}
