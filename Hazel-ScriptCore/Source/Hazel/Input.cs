
namespace Hazel
{
	public class Input
	{
		public static bool IsKeydown(KeyCode keycode)
		{
			return InternalCalls.Input_IsKeydown(keycode);
		}
	}
}
