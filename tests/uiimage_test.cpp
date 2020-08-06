#include "Monochrome.h"
using namespace mc;

#pragma comment(lib, "wininet.lib")
#include <WinInet.h>

int main()
{
	auto window = UIWindow::Create(WindowStyle::Modern, 1220, 560, "UIImage Test");
	window->SetBackgroundColor(Color(26, 21, 22, 1.0f));
	window->SetModernWindowButtonsColor(window->GetBackgroundColor());

	Ref<UIImage> WebImage1 = MakeRef<UIImage>(Frame(100, 100, 500, 300));
	WebImage1->LoadWebImage("http://wallpapers-images.ru/1920x1080/nature/wallpapers/wallpapers-nature-1.jpg");
	if (!WebImage1->IsImageLoaded())
		printf("Image 1 was not successfully loaded from the web\n");

	window->AddView(WebImage1);

	Ref<UIImage> WebImage2 = MakeRef<UIImage>(Frame(640, 100, 500, 300));
	WebImage2->LoadWebImage("http://wallpapers-images.ru/1920x1080/nature/wallpapers/wallpapers-nature-3.jpg");
	if (!WebImage2->IsImageLoaded())
		printf("Image 2 was not successfully loaded from the web\n");

	window->AddView(WebImage2);

	window->StartWindowLoop();
	return 0;
}