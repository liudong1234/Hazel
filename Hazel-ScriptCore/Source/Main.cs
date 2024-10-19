using System;

namespace Hazel
{
	public class Main
	{
		public float FloatVar { get; set; }

		public Main()
		{
			Console.WriteLine("Main constructor");
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
	}

}
