using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Hazel;

namespace Sandbox
{
	class Camera : Entity
	{
		public Entity OtherEntitiy;
		public float DistanceFromPlayer = 4.0f;
		private Entity playerEntity;
		
		public void OnCreate()
		{
			playerEntity = FindEntityByName("Player");
			Translation = new Vector3(Translation.XY, DistanceFromPlayer);
		}

		public void OnUpdate(float ts)
		{
			Player player = playerEntity.As<Player>();
			if (player != null)
			{
				Translation = new Vector3(player.Translation.XY, DistanceFromPlayer);
			}

			//Console.WriteLine($"Player.OnUpdate {ts}");
			float speed = 1.0f;
			Vector3 velocity = Vector3.Zero;

			if (Input.IsKeydown(KeyCode.Left))
				velocity.X -= 1.0f;
			else if (Input.IsKeydown(KeyCode.Right))
				velocity.X += 1.0f;

			if (Input.IsKeydown(KeyCode.Up))
				velocity.Y -= 1.0f;
			else if (Input.IsKeydown(KeyCode.Down))
				velocity.Y += 1.0f;

			velocity = velocity * speed;


			Vector3 translation = Translation;
			translation += velocity * ts;
			Translation = translation;
		}
	}
}
