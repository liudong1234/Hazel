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
        Hazelnut(ApplcationSpecification& spec):
			Application(spec)
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
		ApplcationSpecification spec;
		spec.Name = "Hazelnut";
		spec.CommandLineArgs = args;
        return new Hazelnut(spec);
    }
}
