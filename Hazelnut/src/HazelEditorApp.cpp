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
        Hazelnut(ApplicationCommandLineArgs args):
            Application("Hazelnut", args)
        {
            //PushLayer(new ExampleLayer());
            PushLayer(new EditorLayer());
        }
        ~Hazelnut()
        {

        }
    };

    Application* CreateApplication(ApplicationCommandLineArgs args)
    {
        return new Hazelnut(args);
    }
}