#pragma once

//入口
//将沙箱入口点，移入了Hazel内部
#ifdef HZ_PLATFORM_WINDOW

extern Hazel::Application* Hazel::CreateApplication();

int main()
{
<<<<<<< HEAD
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
=======
	Hazel::Log::Init();
	HZ_PROFILE_BEGIN_SESSION("Creating", "create.json");
	auto app = Hazel::CreateApplication();
	HZ_PROFILE_END_SESSION();
	HZ_PROFILE_BEGIN_SESSION("Running", "run.json");
	app->Run();
	HZ_PROFILE_END_SESSION();

	delete app;

	return 0;
>>>>>>> 105b83b5a7f24a67807d82b5cf407cf934ec75b7
}
#endif