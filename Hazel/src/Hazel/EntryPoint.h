#pragma once

//入口
//将沙箱入口点，移入了Hazel内部
#ifdef HZ_PLATFORM_WINDOW

extern Hazel::Application* Hazel::CreateApplication();

int main()
{
	Hazel::Log::Init();
	HZ_CORE_WARN("hahah");
	HZ_INFO("dslfjafas;fj");

	auto app = Hazel::CreateApplication();
	app->Run();
	delete app;

	return 0;
}
#endif