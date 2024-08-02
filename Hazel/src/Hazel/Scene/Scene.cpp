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

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { this->m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
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
		Renderer2D::BeginScene(camera);
		auto group = this->m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			//Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
			Renderer2D::DrawSprite(transform.GetTransform(), sprite, int(entity));
		}
		Renderer2D::EndScene();
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
            auto group = this->m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
            for (auto entity : group)
            {
                auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
				Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
                //Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
            }
            Renderer2D::EndScene();

        }
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
		}

	}

	void Scene::OnUpdateStop()
	{
		delete this->m_PhysicsWorld;
		this->m_PhysicsWorld = nullptr;

	}

    template<typename T>
    void Scene::OnComponentAdded(Entity entity, T& component)
    {
        static_assert(false);
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
}
