using System;
using System.Runtime.CompilerServices;

namespace Hazel
{
	public static class InternalCalls
	{
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static bool Entity_HasComponent(ulong entityID, Type componentType);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static float TransformComponent_GetTranslation(ulong entityID, out Vector3 translation);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static float TransformComponent_SetTranslation(ulong entityID, ref Vector3 translation);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static float RigidbodyComponent_ApplyLinearImpulse(ulong entityID, ref Vector2 impulse, ref Vector2 worldPosition, bool wake);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static float RigidbodyComponent_ApplyLinearImpulseToCenter(ulong entityID, ref Vector2 impulse, bool wake);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static bool Input_IsKeydown(KeyCode keycode);
		//test
		/*[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void NativeLog(string str, int param);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void Native_Vector(ref Vector3 vec, out Vector3 result);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static float Native_VectorDot(ref Vector3 vec);*/
	}
}
