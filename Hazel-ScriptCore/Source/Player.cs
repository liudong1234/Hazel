using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Hazel;

namespace Sandbox
{
	class Player : Entity
	{
		public void OnCreate()
		{
			Console.WriteLine($"Player OnCreate {ID}");
		}

		public void OnUpdate(float ts)
		{
			Console.WriteLine($"Player.OnUpdate {ts}");
			float speed = 1.0f;
			// Vector3 translation = Translation;
			Vector3 velocity = Vector3.Zero;

			if (Input.IsKeydown(KeyCode.A))
				velocity.X -= 0.1f;
			else if (Input.IsKeydown(KeyCode.D))
				velocity.X += 0.1f;

			if (Input.IsKeydown(KeyCode.S))
				velocity.Y -= 0.1f;
			else if (Input.IsKeydown(KeyCode.W))
				velocity.Y += 0.1f;

			Vector3 translation = Translation;

			velocity = velocity * speed;
			translation += velocity;
			Translation = translation;

		}
	}
}
