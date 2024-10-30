using System;
using System.Runtime.CompilerServices;

namespace Hazel
{
	public struct Vector3
	{
		public float X, Y, Z;

		public Vector3(float x, float y, float z)
		{
			X = x;
			Y = y;
			Z = z;
		}
	}
	public static class InternalCalls
	{
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void NativeLog(string str, int param);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void Native_Vector(ref Vector3 vec, out Vector3 result);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static float Native_VectorDot(ref Vector3 vec);
	}
	public class Main
	{
		public float FloatVar { get; set; }

		

		public Main()
		{
			Console.WriteLine("Main constructor");
			InternalCalls.NativeLog("wangle", 1024);

			Vector3 vec = new Vector3(5, 2.5f, 1);
			Vector3 temp = Log(vec);
			Console.WriteLine($"{temp.X}, {temp.Y}, {temp.Z}");

			float value = InternalCalls.Native_VectorDot(ref vec);
			Console.WriteLine($"{value}");

		}

		public void PrintMessage()
		{
			Console.WriteLine("Hello world from C#");
		}

		public void PrintInts(int a, int b)
		{
			Console.WriteLine($"C# says: {a} and {b}");
		}

		public void PrintMessage(string message)
		{
			Console.WriteLine($"C# says: {message}");
		}

		public Vector3 Log(Vector3 vec)
		{
			InternalCalls.Native_Vector(ref vec, out Vector3 result);
			return result;
		}

	}

}
