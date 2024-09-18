#include "hzpch.h"
#include "Scene.h"
#include "Components.h"
#include "Hazel/Renderer/Renderer2D.h"
#include <glm/glm.hpp>

//Box2D
#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"
#include "box2d/b2_circle_shape.h"

namespace Hazel
{
	static b2BodyType RigidBody2DTypeTOBox2DBody(RigidBody2DComponent::BodyType bodyType)
	{
		switch (bodyType)
		{
			case RigidBody2DComponent::BodyType::Static:
				return b2BodyType::b2_staticBody;
				break;
			case RigidBody2DComponent::BodyType::Dynamic:
				return b2BodyType::b2_dynamicBody;
				break;
			case RigidBody2DComponent::BodyType::Kinematic:
				return b2BodyType::b2_kinematicBody;
				break;
			default:
				break;
		}
		HZ_CORE_ASSERT(false, "Unknown BodyType");
		return b2BodyType::b2_staticBody;

	}

	Scene::Scene() :
		m_ViewportHeight(0), m_ViewportWidth(0),
		m_PhysicsWorld(nullptr)
	{
	}

	Scene::~Scene()
	{

	}

	template<typename... Component>
	static void CopyComponent(entt::registry& srcRegistry, entt::registry& dstRegistry, const std::unordered_map<UUID, entt::entity>& enttMap)
	{
		([&]()
			{
				auto srcView = srcRegistry.view<Component>();
				for (auto& en : srcView)
				{
					auto& srcuuid = srcRegistry.get<IDComponent>(en).ID;
					HZ_CORE_ASSERT(enttMap.find(srcuuid) != enttMap.end(), "");
					entt::entity descEnttId = enttMap.at(srcuuid);

					auto& component = srcRegistry.get<Component>(en);
					dstRegistry.emplace_or_replace<Component>(descEnttId, component);


					//auto& descView = dstRegistry.view<Component>();
					/*for (auto& de : descView)
					{
						auto& descuuid = descRegistry.get<IDComponent>(en).ID;
						if (srcuuid == descuuid)
							descRegistry.emplace_or_replace<Component>(de, component);
					}*/

				}
			}(), ...);
	}

	template<typename... Component>
	static void CopyComponent(ComponentGroup<Component...>, entt::registry& srcRegistry, entt::registry& dstRegistry, const std::unordered_map<UUID, entt::entity>& enttMap)
	{
		CopyComponent<Component...>(srcRegistry, dstRegistry, enttMap);
	}

	template<typename... Component>
	static void CopyComponentIfExist(Entity& src, Entity& dst)
	{
		([&]()
			{
				if (src.HasComponent<Component>())
				{
					dst.AddOrReplaceComponent<Component>(src.GetComponent<Component>());
				}
			}(), ...);
	}

	template<typename... Component>
	static void CopyComponentIfExist(ComponentGroup<Component...>, Entity& src, Entity& dst)
	{
		CopyComponentIfExist<Component...>(src, dst);
	}

	Ref<Scene> Scene::Copy(Ref<Scene> other)
	{
		Ref<Scene> newScene = CreateRef<Scene>();
		newScene->m_ViewportWidth = other->m_ViewportWidth;
		newScene->m_ViewportHeight = other->m_ViewportHeight;

		auto& srcSceneRegistry = other->m_Registry;
		auto& dstSceneRegistry = newScene->m_Registry;
		auto idView = srcSceneRegistry.view<IDComponent>();
		std::unordered_map<UUID, entt::entity> enttMap;

		for (auto en : idView)
		{
			UUID uuid = srcSceneRegistry.get<IDComponent>(en).ID;
			const auto& name = srcSceneRegistry.get<TagComponent>(en).Tag;
			Entity entity = newScene->CreateEntityWithUUID(uuid, name);
			enttMap[uuid] = (entt::entity)entity;
		}


		CopyComponent(AllComponents{}, srcSceneRegistry, dstSceneRegistry, enttMap);

		//CopyComponent<TransformComponent>(srcSceneRegistry, dstSceneRegistry, enttMap);
		//CopyComponent<CameraComponent>(srcSceneRegistry, dstSceneRegistry, enttMap);
		//CopyComponent<SpriteRendererComponent>(srcSceneRegistry, dstSceneRegistry, enttMap);
		//CopyComponent<CircleRendererComponent>(srcSceneRegistry, dstSceneRegistry, enttMap);
		//CopyComponent<NativeScriptComponent>(srcSceneRegistry, dstSceneRegistry, enttMap);
		//CopyComponent<RigidBody2DComponent>(srcSceneRegistry, dstSceneRegistry, enttMap);
		//CopyComponent<BoxCollider2DComponent>(srcSceneRegistry, dstSceneRegistry, enttMap);
		//CopyComponent<CircleCollider2DComponent>(srcSceneRegistry, dstSceneRegistry, enttMap);

		return newScene;
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		return CreateEntityWithUUID(UUID(), name);
	}

	Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string& name)
	{
		Entity entity = { this->m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<IDComponent>(uuid);
		auto& tag = entity.AddComponent<TagComponent>();
		tag = name.empty() ? "Entity" : name;

		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		this->m_Registry.destroy(entity);
	}


	void Scene::OnUpdateEditor(TimeStep ts, EditorCamera& camera)
	{
		this->RenderScene(camera);
	}

    void Scene::OnUpdateRuntime(TimeStep ts)
    {
        //update scripts
        {
            this->m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
                {
                    if (!nsc.Instance)
                    {
                        nsc.Instance = nsc.InstantiateScript();
                        nsc.Instance->m_Entity = Entity{ entity, this };
                        nsc.Instance->OnCreate();
                    }
                    nsc.Instance->OnUpdate(ts);
                });

        }

		//Physics
		{
			const int32_t velocityIterations = 6;
			const int32_t positionIterations = 2;
			this->m_PhysicsWorld->Step(ts, velocityIterations, positionIterations);
			//重新从Box2D得到transform
			auto view = this->m_Registry.view<RigidBody2DComponent>();
			for (auto& en : view )
			{
				Entity  entity = { en, this };
				auto& transform = entity.GetComponent<TransformComponent>();
				auto& rb2d = entity.GetComponent<RigidBody2DComponent>();

				b2Body* body = (b2Body*)rb2d.RuntimeBody;
				const auto& position = body->GetPosition();
				transform.Translation.x = position.x;
				transform.Translation.y = position.y;
				transform.Rotation.z = body->GetAngle();
			}
		}

        //render sprites
        Camera* mainCamera = nullptr;
        glm::mat4 cameraTransform;
        {
            auto views = this->m_Registry.view<TransformComponent, CameraComponent>();
            for (auto entity : views)
            {
                //auto& transform = views.get<TransformComponent>(entity);
                //auto& camera = views.get<CameraComponent>(entity);
                auto [transform, camera] = views.get<TransformComponent, CameraComponent>(entity);
                
                if (camera.Primary)
                {
                    mainCamera = &camera.Camera;
                    cameraTransform = transform.GetTransform();
                    break;
                }
            
            }
        }
        if (mainCamera)
        {
            Renderer2D::BeginScene(*mainCamera, cameraTransform);
			{
				auto group = this->m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
				for (auto entity : group)
				{
					auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
					Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
					//Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
				}
			}

			//Draw Circle
			{
				auto view = this->m_Registry.view<TransformComponent, CircleRendererComponent>();
				for (auto entity : view)
				{
					auto [transform, circle] = view.get<TransformComponent, CircleRendererComponent>(entity);

					//Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
					Renderer2D::DrawCircle(transform.GetTransform(), circle.Color, circle.Thickness, circle.Fade, int(entity));
				}
			}
            Renderer2D::EndScene();
        }
    }

	void Scene::OnUpdateSimulation(TimeStep ts, EditorCamera& camera)
	{
		//physics
		{
			const int32_t velocityIterations = 6;
			const int32_t positionIterations = 2;
			this->m_PhysicsWorld->Step(ts, velocityIterations, positionIterations);
			//重新从Box2D得到transform
			auto view = this->m_Registry.view<RigidBody2DComponent>();
			for (auto& en : view)
			{
				Entity  entity = { en, this };
				auto& transform = entity.GetComponent<TransformComponent>();
				auto& rb2d = entity.GetComponent<RigidBody2DComponent>();

				b2Body* body = (b2Body*)rb2d.RuntimeBody;
				const auto& position = body->GetPosition();
				transform.Translation.x = position.x;
				transform.Translation.y = position.y;
				transform.Rotation.z = body->GetAngle();
			}
		}

		this->RenderScene(camera);
	}
    
	void Scene::OnViewportResize(uint32_t width, uint32_t height)
    {
        this->m_ViewportWidth = width;
        this->m_ViewportHeight = height;

        auto view = this->m_Registry.view<CameraComponent>();
        for (auto entity : view)
        {
            auto& cameraComponent = view.get<CameraComponent>(entity);
            if (!cameraComponent.FixedAspectRatio)
            {
                cameraComponent.Camera.SetViewportSize(width, height);
            }
        }
    }

	void Scene::DuplicateEntity(Entity entity)
	{
		Entity newEntity = CreateEntity(entity.GetName());

		CopyComponentIfExist(AllComponents{}, entity, newEntity);

		CopyComponentIfExist<TransformComponent>(entity, newEntity);
		CopyComponentIfExist<CameraComponent>(entity, newEntity);
		CopyComponentIfExist<SpriteRendererComponent>(entity, newEntity);
		CopyComponentIfExist<CircleRendererComponent>(entity, newEntity);
		CopyComponentIfExist<NativeScriptComponent>(entity, newEntity);
		CopyComponentIfExist<RigidBody2DComponent>(entity, newEntity);
		CopyComponentIfExist<BoxCollider2DComponent>(entity, newEntity);
		CopyComponentIfExist<CircleCollider2DComponent>(entity, newEntity);

	}

	Entity Scene::GetPrimaryCamera()
	{
		auto views = this->m_Registry.view<CameraComponent>();
		for (auto entity : views)
		{
			const auto& cc = this->m_Registry.get<CameraComponent>(entity);
			if (cc.Primary)
				return Entity{ entity, this };
		}

		return {};
	}

	void Scene::OnUpdateStart()
	{
		this->OnPhysics2DStart();
	}

	void Scene::OnUpdateStop()
	{
		this->OnPhysics2DStop();
	}

	void Scene::OnSimulationStart()
	{
		this->OnPhysics2DStart();
	}

	void Scene::OnSimulationStop()
	{
		this->OnPhysics2DStop();
	}


	void Scene::RenderScene(EditorCamera& camera)
	{
		Renderer2D::BeginScene(camera);
		//Draw Quad
		{
			auto group = this->m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				//Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
				Renderer2D::DrawSprite(transform.GetTransform(), sprite, int(entity));
				//Renderer2D::DrawRect(transform.GetTransform(), sprite.Color, int(entity));
			}
		}
		//Draw Circle
		{
			auto view = this->m_Registry.view<TransformComponent, CircleRendererComponent>();
			for (auto entity : view)
			{
				auto [transform, circle] = view.get<TransformComponent, CircleRendererComponent>(entity);

				//Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
				Renderer2D::DrawCircle(transform.GetTransform(), circle.Color, circle.Thickness, circle.Fade, int(entity));
			}
		}

		/*{
			Renderer2D::DrawLine(glm::vec3(0.0f), glm::vec3(4.0f), glm::vec4(1.0f, 0.0f, 0.34f, 1.0f));
			Renderer2D::DrawRect(glm::vec3(0.0f), glm::vec2(3.0f, 3.0f), glm::vec4(1.0f));
		}*/

		Renderer2D::EndScene();
	}

	void Scene::OnPhysics2DStart()
	{
		this->m_PhysicsWorld = new b2World({ 0.0f, -9.8f });
		auto view = m_Registry.view<RigidBody2DComponent>();
		for (auto& en : view)
		{
			Entity entity = { en, this };
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& rb2d = entity.GetComponent<RigidBody2DComponent>();

			b2BodyDef bodydef;
			bodydef.type = RigidBody2DTypeTOBox2DBody(rb2d.Type);
			bodydef.position.Set(transform.Translation.x, transform.Translation.y);
			bodydef.angle = transform.Rotation.z;

			b2Body* body = this->m_PhysicsWorld->CreateBody(&bodydef);
			body->SetFixedRotation(rb2d.FixedRotation);
			rb2d.RuntimeBody = body;

			if (entity.HasComponent<BoxCollider2DComponent>())
			{
				auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();
				b2PolygonShape polygonShape;
				polygonShape.SetAsBox(bc2d.Size.x * transform.Scale.x, bc2d.Size.y * transform.Scale.y);

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &polygonShape;
				fixtureDef.density = bc2d.Density;
				fixtureDef.friction = bc2d.Friction;
				fixtureDef.restitution = bc2d.Restitution;
				fixtureDef.restitutionThreshold = bc2d.RestitutionThreshold;
				body->CreateFixture(&fixtureDef);
			}

			if (entity.HasComponent<CircleCollider2DComponent>())
			{
				auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();
				b2CircleShape circleShape;
				circleShape.m_p.Set(cc2d.Offset.x, cc2d.Offset.y);
				circleShape.m_radius = transform.Scale.x * cc2d.Radius;

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &circleShape;
				fixtureDef.density = cc2d.Density;
				fixtureDef.friction = cc2d.Friction;
				fixtureDef.restitution = cc2d.Restitution;
				fixtureDef.restitutionThreshold = cc2d.RestitutionThreshold;
				body->CreateFixture(&fixtureDef);
			}
		}
	}

	void Scene::OnPhysics2DStop()
	{
		delete this->m_PhysicsWorld;
		this->m_PhysicsWorld = nullptr;
	}

    template<typename T>
    void Scene::OnComponentAdded(Entity entity, T& component)
    {
        static_assert(sizeof(T) == );
    }

	template<>
	void Scene::OnComponentAdded<IDComponent>(Entity entity, IDComponent& component)
	{
	}
    
	template<>
    void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
    {

    }
    
	template<>
    void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
    {

    }
	
	template<>
	void Scene::OnComponentAdded<CircleRendererComponent>(Entity entity, CircleRendererComponent& component)
	{

	}
    
	template<>
    void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
    {
        if (this->m_ViewportWidth > 0 && this->m_ViewportHeight > 0)
            component.Camera.SetViewportSize(this->m_ViewportWidth, this->m_ViewportHeight);
    }
    
	template<>
    void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
    {

    }
    
	template<>
    void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
    {

    }

	template<>
	void Scene::OnComponentAdded<RigidBody2DComponent>(Entity entity, RigidBody2DComponent& component)
	{

	}
	
	template<>
	void Scene::OnComponentAdded<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& component)
	{

	}
	
	template<>
	void Scene::OnComponentAdded<CircleCollider2DComponent>(Entity entity, CircleCollider2DComponent& component)
	{

	}
}
