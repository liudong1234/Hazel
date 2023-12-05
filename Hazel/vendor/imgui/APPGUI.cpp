#include"APPGUI.h"
#include "imgui.h"


void MyApp::app()
{
	ImGui::Begin("the third window");                          // Create a window called "Hello, world!" and append into it.

	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

	ImGui::Button("buton");


	ImGui::End();
}
