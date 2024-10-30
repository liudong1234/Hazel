#include "hzpch.h"

#include "ScriptGlue.h"
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

	void ScriptGlue::RegisterFunctions()
	{
		HZ_ADD_INTERNAL_CALL(NativeLog);
		HZ_ADD_INTERNAL_CALL(Native_Vector);
		HZ_ADD_INTERNAL_CALL(Native_VectorDot);
	}
}
