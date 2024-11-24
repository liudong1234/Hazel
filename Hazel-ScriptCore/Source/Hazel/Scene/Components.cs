using System;

namespace Hazel
{
	public abstract class Component
	{
		public Entity Entity { get; internal set; }
	}

	class TransformComponent: Component
	{
		public static int ID = 1;
		//public Vector3 Translation;
		public Vector3 Translation
		{
			get
			{
				InternalCalls.TransformComponent_GetTranslation(Entity.ID, out Vector3 translation);
				return translation;
			}
			set
			{
				InternalCalls.TransformComponent_SetTranslation(Entity.ID, ref value);
			}
		}

	}

	class RigidBody2DComponent : Component
	{
		public void ApplyLinerImpulse(Vector2 impulse, Vector2 worldPosition, bool wake)
		{
			InternalCalls.RigidbodyComponent_ApplyLinearImpulse(Entity.ID, ref impulse, ref worldPosition, wake);
		}

		public void ApplyLinerImpulse(Vector2 impulse, bool wake)
		{
			InternalCalls.RigidbodyComponent_ApplyLinearImpulseToCenter(Entity.ID, ref impulse, wake);
		}

	}

}
