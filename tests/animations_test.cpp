#include <Monochrome.h>
using namespace mc;

int main()
{
    auto window = UIWindow::Create(WindowStyle::Modern, 400, 660, "Animations Test");
	window->SetBackgroundColor(Color(6, 47, 103, 1));
	window->SetModernWindowButtonsColor(Color(6, 47, 103, 1));

    UITimer timer([](uint32_t tick, UITimer* timer) {
        printf("Tick: %u\n", tick);
        if (tick == 10)
            timer->Stop();
    });

    timer.Start(500);

    window->StartWindowLoop();
    return 0;
}
