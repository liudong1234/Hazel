using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Hazel;

namespace Sandbox
{
	public class Player : Entity
	{
		private TransformComponent m_Transform;
		private RigidBody2DComponent m_Rigidbody;

		public float Speed;
		public float time;

		public void OnCreate()
		{
			Console.WriteLine($"Player OnCreate {ID}");
			m_Transform = GetComponent<TransformComponent>();
			m_Rigidbody = GetComponent<RigidBody2DComponent>();
			//Console.WriteLine("{0}{1}{2}", m_Transform.Translation.X, m_Transform.Translation.Y, m_Transform.Translation.Z);
			time = 0.0f;
		}

		public void OnUpdate(float ts)
		{
			//Console.WriteLine($"Player.OnUpdate {ts}");
			time += ts;
			float speed = Speed;
			Vector3 velocity = Vector3.Zero;
		
			if (Input.IsKeydown(KeyCode.A))
				velocity.X -= 1.0f;
			else if (Input.IsKeydown(KeyCode.D))
				velocity.X += 1.0f;

			if (Input.IsKeydown(KeyCode.S))
				velocity.Y -= 1.0f;
			else if (Input.IsKeydown(KeyCode.W))
				velocity.Y += 1.0f;

			Entity cameraEntity = FindEntityByName("Camera");
			if (cameraEntity != null)
			{
				Camera camera = cameraEntity.As<Camera>();

				if (Input.IsKeydown(KeyCode.Q))
				{
					camera.DistanceFromPlayer += speed * ts;
				}
				if (Input.IsKeydown(KeyCode.E))
				{
					camera.DistanceFromPlayer -= speed * ts;
				}

			}
			velocity *= speed;

			m_Rigidbody.ApplyLinerImpulse(velocity.XY, true);

			/*Vector3 translation = m_Transform.Translation;
			translation += velocity * ts;
			m_Transform.Translation = translation;*/

		}
	}
}
