#include "hzpch.h"

#include "Hazel/Core/Input.h"
#include "Hazel/Core/KeyCodes.h"
#include "ScriptGlue.h"
#include "ScriptEngine.h"
#include "Hazel/Core/UUID.h"
#include <mono/metadata/object.h>

namespace Hazel
{
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

	static void Entity_GetTranslation(UUID entityID, glm::vec3* outTranslation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(entityID);

		*outTranslation = entity.GetComponent<TransformComponent>().Translation;
	}

	static void Entity_SetTranslation(UUID entityID, glm::vec3* translation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(entityID);

		entity.GetComponent<TransformComponent>().Translation = *translation;
	}

	static bool Input_IsKeydown(KeyCode keycode)
	{
		return Input::IsKeyPressed(keycode);
	}

	void ScriptGlue::RegisterFunctions()
	{
		HZ_ADD_INTERNAL_CALL(NativeLog);
		HZ_ADD_INTERNAL_CALL(Native_Vector);
		HZ_ADD_INTERNAL_CALL(Native_VectorDot);
		HZ_ADD_INTERNAL_CALL(Entity_GetTranslation);
		HZ_ADD_INTERNAL_CALL(Entity_SetTranslation);
		HZ_ADD_INTERNAL_CALL(Input_IsKeydown);
	}
}
