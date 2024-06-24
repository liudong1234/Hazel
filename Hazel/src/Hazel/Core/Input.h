#pragma
#include "hzpch.h"
#include "Core.h"


namespace Hazel
{
<<<<<<< HEAD
    class HAZEL_API Input
    {
    public:
        static bool IsKeyPressed(int keycode);
        static bool IsMouseButtonPressed(int keycode);

        static std::pair<float, float> GetMousePosition();

        static float GetMouseX();
        static float GetMouseY();

    //protected:
    //  virtual bool IsKeyPressedImpl(int keycode) = 0;
    //  virtual bool IsMouseButtonPressedImpl(int keycode) = 0;
    //  virtual std::pair<float, float> GetMousePositionImpl() = 0;
    //  virtual float GetMouseXImpl() = 0;
    //  virtual float GetMouseYImpl() = 0;

    //private:
    //  static Input* s_Instance;
    };
=======
	class HAZEL_API Input
	{
	public:
		inline static bool IsKeyPressed(int keycode) { return s_Instance->IsKeyPressedImpl(keycode); }
		inline static bool IsMouseButtonPressed(int keycode) { return s_Instance->IsMouseButtonPressedImpl(keycode); }

		inline static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }

		inline static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
		inline static float GetMouseY() { return s_Instance->GetMouseYImpl(); }

	protected:
		virtual bool IsKeyPressedImpl(int keycode) = 0;
		virtual bool IsMouseButtonPressedImpl(int keycode) = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;

	private:
		static Input* s_Instance;
	};
>>>>>>> 105b83b5a7f24a67807d82b5cf407cf934ec75b7

}