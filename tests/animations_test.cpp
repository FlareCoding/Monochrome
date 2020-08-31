#include <Monochrome.h>
using namespace mc;

Ref<UICircularProgressBar> cpb;
Ref<UILabel> label;

void ProgressBar_ValueChanged(float value, UICircularProgressBar* sender)
{
    if (value == 100.0f)
    {
        auto fadeout = Animation::Create(AnimationType::FadeOut, sender);
        fadeout->Animate(100, []() {
            auto label_fadein = Animation::Create(AnimationType::FadeIn, label.get());
            label_fadein->Animate(1200);
        });
    }
}

bool StartAnimBtn_OnClick(Event& event, UIView* sender)
{
    auto translate_down = Animation::Create(AnimationType::Translation, sender);
    AnimCast(TranslationAnimation, translate_down)->SetTargetTranslation({ 0, 320 });
    translate_down->Animate(900, [sender]() {
        auto fadeout = Animation::Create(AnimationType::FadeOut, sender);
        fadeout->Animate(200);

        auto scaleup = Animation::Create(AnimationType::Scale, cpb.get());
        AnimCast(ScaleAnimation, scaleup)->SetTargetSize({ 100.0f, 100.0f });
        scaleup->Animate(200, []() {
            auto cpb_anim = Animation::Create(AnimationType::Custom, 0);
            AnimCast(CustomAnimation, cpb_anim)->SetAnimationFunction([](uint32_t tick) {
                std::this_thread::sleep_for(std::chrono::milliseconds(16));
                cpb->Value++;
            });
            cpb_anim->Animate(100);
        });
    });

    return EVENT_HANDLED;
}

int main()
{
    auto window = UIWindow::Create(WindowStyle::Modern, 400, 660, "Animations Test");
	window->SetBackgroundColor(Color(6, 47, 103, 1));
	window->SetModernWindowButtonsColor(Color(46, 147, 203, 1));

    auto button = MakeRef<UIButton>(Frame(120, 200, 160, 50));
    button->layer.color = Color(16, 27, 63, 1.0f);
    button->Label->Text = "Start";
    button->Label->color = Color::white;
    button->AddEventHandler<EventType::MouseButtonClicked>(StartAnimBtn_OnClick);
    window->AddView(button);

    cpb = MakeRef<UICircularProgressBar>(Frame(150, 200, 0, 0));
    cpb->layer.color = Color(16, 17, 13, 1.0f);
    cpb->ProgressColor = Color::green;
    cpb->Stroke = 6.0f;
    cpb->Value = 0;
    cpb->AddValueChangedEventHandler(ProgressBar_ValueChanged);
    window->AddView(cpb);
    
    label = MakeRef<UILabel>(Frame(100, 200, 200, 50));
    label->color = Color::white;
    label->layer.color.alpha = 0.0f;
    label->Visible = false;
    label->Properties.FontSize = 20.0f;
    label->Text = "Animation Finished";
    window->AddView(label);

    window->StartWindowLoop();
    return 0;
}
