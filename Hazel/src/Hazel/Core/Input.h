#pragma
#include "hzpch.h"
#include "Core.h"


namespace Hazel
{
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

}