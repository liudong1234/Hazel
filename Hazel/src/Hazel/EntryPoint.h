#pragma once

//入口
#ifdef HZ_PLATFORM_WINDOW

extern Hazel::Application* Hazel::CreateApplication();

int main()
{
	auto app = Hazel::CreateApplication();
	app->Run();
	delete app;

	return 0;
}
#endif