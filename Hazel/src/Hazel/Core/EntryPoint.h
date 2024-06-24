#pragma once

//入口
//将沙箱入口点，移入了Hazel内部
#ifdef HZ_PLATFORM_WINDOW

extern Hazel::Application* Hazel::CreateApplication();

int main()
{
    Hazel::Log::Init();
    HZ_PROFILE_BEGIN_SESSION("Startup", "HazelProfile-Startup.json");
    auto app = Hazel::CreateApplication();
    HZ_PROFILE_END_SESSION();

    HZ_PROFILE_BEGIN_SESSION("Runtime", "HazelProfile-Runtime.json");
    app->Run();
    HZ_PROFILE_END_SESSION();

    HZ_PROFILE_BEGIN_SESSION("Shutdown", "HazelProfile-Shutdown.json");
    delete app;
    HZ_PROFILE_END_SESSION();

    return 0;
}
#endif