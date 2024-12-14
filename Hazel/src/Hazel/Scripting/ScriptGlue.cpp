#include "hzpch.h"
#include "ScriptGlue.h"
#include "ScriptEngine.h"

#include "Hazel/Core/UUID.h"
#include "Hazel/Core/Input.h"
#include "Hazel/Core/KeyCodes.h"
#include "box2d/b2_body.h"


#include <mono/metadata/object.h>
#include <mono/metadata/reflection.h>

namespace Hazel
{
	std::unordered_map<MonoType*, std::function<bool(Entity)>> s_EntityHasComponentFuncs;


	#define HZ_ADD_INTERNAL_CALL(Name) mono_add_internal_call("Hazel.InternalCalls::" #Name, Name)

	static void NativeLog(MonoString* monoStr, int param)
	{
		char* str = mono_string_to_utf8(monoStr);
		HZ_CORE_WARN("{}, says {}", str, param);
		mono_free(str);
		HZ_CORE_WARN("Hello world from c++\n");
	}

	static void Native_Vector(glm::vec3* vec, glm::vec3* outvec)
	{
		HZ_CORE_WARN("{0}", *vec);

		*outvec = glm::cross(*vec, glm::vec3(vec->x, vec->y, -vec->z));
	}

	static float Native_VectorDot(glm::vec3* vec)
	{
		return glm::dot(*vec, *vec);
	}

	static void TransformComponent_GetTranslation(UUID entityID, glm::vec3* outTranslation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(entityID);

		*outTranslation = entity.GetComponent<TransformComponent>().Translation;
	}

	static void TransformComponent_SetTranslation(UUID entityID, glm::vec3* translation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(entityID);

		entity.GetComponent<TransformComponent>().Translation = *translation;
	}

	static bool Input_IsKeydown(KeyCode keycode)
	{
		return Input::IsKeyPressed(keycode);
	}

	static bool Entity_HasComponent(UUID entityID, MonoReflectionType* componentType)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		HZ_CORE_ASSERT(scene, "scene is empty");

		Entity entity = scene->GetEntityByUUID(entityID);
		HZ_CORE_ASSERT(entity, "entity is null");

		MonoType* managedType = mono_reflection_type_get_type(componentType);
		HZ_CORE_ASSERT(managedType, "managedType is null");

		return s_EntityHasComponentFuncs.at(managedType)(entity);

	}

	static void RigidbodyComponent_ApplyLinearImpulse(UUID entityID, glm::vec2* impulse, glm::vec2* point, bool wake)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		HZ_CORE_ASSERT(scene, "scene is empty");

		Entity entity = scene->GetEntityByUUID(entityID);
		HZ_CORE_ASSERT(entity, "entity is null");

		auto& rb2d = entity.GetComponent<RigidBody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		body->ApplyLinearImpulse(b2Vec2(impulse->x, impulse->y), b2Vec2(point->x, point->y), wake);
	}

	static void RigidbodyComponent_ApplyLinearImpulseToCenter(UUID entityID, glm::vec2* impulse, bool wake)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		HZ_CORE_ASSERT(scene, "scene is empty");

		Entity entity = scene->GetEntityByUUID(entityID);
		HZ_CORE_ASSERT(entity, "entity is null");

		auto& rb2d = entity.GetComponent<RigidBody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		body->ApplyLinearImpulseToCenter(b2Vec2(impulse->x, impulse->y), wake);
	}


	template<typename... Component>
	static void ReigisterComponent()
	{
		([] {

			std::string_view typeName = typeid(Component).name();
			size_t pos = typeName.find_last_of(":");
			std::string_view structName = typeName.substr(pos + 1);
			std::string managedTypeName = fmt::format("Hazel.{}", structName);

			MonoType* managedType = mono_reflection_type_from_name(managedTypeName.data(), ScriptEngine::GetCoreAssemblyImage());
			if (!managedType)
			{
				HZ_CORE_ERROR("Couldn't fine the Compoent {}", managedTypeName);
				return;
			}
			s_EntityHasComponentFuncs[managedType] = [](Entity entity) { return entity.HasComponent<Component>(); };
		}(), ...);
	}

	template<typename... Component>
	static void ReigisterComponent(ComponentGroup<Component ...>)
	{
		ReigisterComponent<Component ...>();
	}

	void ScriptGlue::RegisterComponents()
	{
		ReigisterComponent(AllComponents{});
		
		/*MonoType* managedType = mono_reflection_type_from_name("Hazel.TransformComponent", ScriptEngine::GetCoreAssemblyImage());

		HZ_CORE_ASSERT(managedType, "managedType is null");
		s_EntityHasComponentFuncs[managedType] = [](Entity entity) { return entity.HasComponent<TransformComponent>(); };		*/

	}

	void ScriptGlue::RegisterFunctions()
	{
		HZ_ADD_INTERNAL_CALL(NativeLog);
		HZ_ADD_INTERNAL_CALL(Native_Vector);
		HZ_ADD_INTERNAL_CALL(Native_VectorDot);

		HZ_ADD_INTERNAL_CALL(Entity_HasComponent);
		HZ_ADD_INTERNAL_CALL(TransformComponent_GetTranslation);
		HZ_ADD_INTERNAL_CALL(TransformComponent_SetTranslation);

		HZ_ADD_INTERNAL_CALL(RigidbodyComponent_ApplyLinearImpulse);
		HZ_ADD_INTERNAL_CALL(RigidbodyComponent_ApplyLinearImpulseToCenter);

		HZ_ADD_INTERNAL_CALL(Input_IsKeydown);
	}
}
