#include "Hazel.h"
#include "Hazel/Core/EntryPoint.h"

#include <string>

#include "EditorLayer.h"

namespace Hazel
{
    class Hazelnut :
        public Application
    {
    public:
        Hazelnut():
            Application("Hazelnut")
        {
            //PushLayer(new ExampleLayer());
            PushLayer(new EditorLayer());
        }
        ~Hazelnut()
        {

        }
    };

    Application* CreateApplication()
    {
        return new Hazelnut();
    }
}