using System;

namespace Hazel
{

	public class Entity
	{
		protected Entity() { ID = 0; }

		internal Entity(ulong entityID)
		{
			ID = entityID;
		}

		public readonly ulong ID;

		public Vector3 Translation
		{
			get
			{
				InternalCalls.TransformComponent_GetTranslation(ID, out Vector3 translation);
				return translation;
			}
			set
			{
				InternalCalls.TransformComponent_SetTranslation(ID, ref value);
			}
		}
		
		public bool HasComponent<T>() where T: Component, new()
		{
			Type componentType = typeof(T);
			return InternalCalls.Entity_HasComponent(ID, componentType);
		}

		public Entity FindEntityByName(string entityName)
		{
			ulong entityID = InternalCalls.Entity_FindEntityByName(entityName);
			return new Entity(entityID);
		}

		public T GetComponent<T>() where T: Component, new()
		{
			if (!HasComponent<T>())
				return null;
			T component = new T() { Entity = this };
			return component;
		}

		public T As<T>() where T: Entity, new()
		{
			object instance = InternalCalls.GetScriptInstance(ID);
			return instance as T;
		}

	}

}
