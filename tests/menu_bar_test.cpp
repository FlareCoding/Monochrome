#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#include <Monochrome.h>
using namespace mc;

int main()
{
	auto window = UIWindow::Create(WindowStyle::Modern, 1000, 670, "MenuBar Test");
	window->SetBackgroundColor(Color(38, 38, 39, 1.0f));
	window->SetModernWindowButtonsColor(Color(38, 38, 39, 1.0f));



	window->StartWindowLoop();
}