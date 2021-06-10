#include "UITextArea.h"
#include <graphics/Graphics.h>

namespace mc
{
    UITextArea::UITextArea()
	{
        layer.frame = Frame(40, 80, 180, 120);
        SetDefaultOptions();
	}

	UITextArea::UITextArea(Frame frame) : UIView(frame)
	{
        SetDefaultOptions();
	}

    void UITextArea::SetDefaultOptions()
    {
        Text = R"(
        Lorem ipsum dolor sit amet, consectetur adipiscing elit. Fusce rutrum aliquam dignissim. Aliquam non iaculis massa, vitae mollis sem. Curabitur ullamcorper libero urna, vitae auctor nibh sodales a. Vestibulum eros lorem, ullamcorper eget odio vitae, auctor interdum arcu. Donec sodales suscipit libero, at laoreet mi placerat non. Aliquam ac erat lobortis, condimentum nisl quis, mattis lorem. Nunc euismod elementum magna eu semper. Suspendisse euismod ultricies purus quis venenatis. Sed sed ultrices arcu, at vestibulum lectus. Vestibulum fermentum vestibulum rhoncus. Cras nisi lacus, scelerisque sed erat non, tempus semper urna. Sed vel pharetra lorem, non fringilla turpis.

        Donec ullamcorper justo id euismod aliquet. Ut interdum mauris quam, quis tristique sem efficitur sit amet. Suspendisse vel mi ligula. Morbi elementum luctus sapien id posuere. Aliquam at odio viverra, tempor nisi eget, dictum erat. Proin venenatis diam dolor, id tempus dolor ultrices blandit. Vivamus a risus velit. Suspendisse volutpat faucibus lorem, eget aliquet eros feugiat lobortis. Curabitur venenatis est velit. Vivamus neque leo, rutrum nec aliquam vitae, sodales tincidunt nisl. Donec tincidunt semper tortor. Sed leo neque, accumsan in mattis vitae, ornare non metus.

        Ut ornare, ex semper luctus auctor, elit justo maximus lacus, ut ultrices tortor dolor quis erat. Nam ut mattis leo. Sed et pulvinar quam. Aliquam erat volutpat. Morbi et consequat nulla. Donec at orci lectus. Ut sit amet libero a nulla gravida feugiat. Nam congue dolor sit amet risus egestas vestibulum. Nunc posuere fermentum magna vitae fringilla. Sed ornare condimentum leo, vel scelerisque eros bibendum at. Phasellus quis diam quis leo iaculis venenatis id at leo. Curabitur neque lectus, vehicula ut blandit eget, scelerisque ac erat. Donec tincidunt metus in aliquet vestibulum.
        )";

        layer.color = Color::white;

        m_ContentPanel = MakeRef<UIScrollPanel>(Frame(0, 0, 0, 0));
        AddSubview(m_ContentPanel);

        m_TextSource = MakeRef<UILabel>(Frame(0, 0, 0, 0));
        Properties.Wrapping = WordWrapping::WORD_WRAP;
        Properties.VerticalAlignment = TextAlignment::LEADING;
        m_ContentPanel->AddChild(m_TextSource);
    }

	void UITextArea::Draw()
	{
        Update();
	}

    void UITextArea::Update()
    {
        // Horizontal text alignment should never be centered
        if (Properties.Alignment == TextAlignment::CENTERED)
            Properties.Alignment = TextAlignment::LEADING;

		// Adjusting text color opacity according to frame's color opacity
		if (TextColor.alpha > layer.color.alpha)
			TextColor.alpha = layer.color.alpha;

        m_ContentPanel->layer.frame.size = layer.frame.size;
        m_ContentPanel->layer.color = layer.color;

        m_TextSource->layer.frame.position.x = LeftMargins;
        m_TextSource->layer.frame.position.y = TopMargins;
        m_TextSource->layer.frame.size.width = layer.frame.size.width - (RightMargins + LeftMargins);
        m_TextSource->color = TextColor;
        m_TextSource->Properties = Properties;

        if (Text != m_PreviousText)
            OnTextChanged();

        m_PreviousText = Text;
    }

    void UITextArea::OnTextChanged()
    {
        m_TextSource->Text = Text;

        auto text_metrics = Graphics::CalculateTextMetrics(
            Text,
            Properties,
            layer.frame.size.width,
            10000000
        );

        m_ContentPanel->ContentView->layer.frame.size.width = layer.frame.size.width;
        m_ContentPanel->ContentView->layer.frame.size.height = text_metrics.Height + BottomMargins + TopMargins + 20;

        m_TextSource->layer.frame.position.x = LeftMargins;
        m_TextSource->layer.frame.position.y = TopMargins;
        m_TextSource->layer.frame.size.width = layer.frame.size.width - (RightMargins + LeftMargins);
        m_TextSource->layer.frame.size.height = text_metrics.Height + BottomMargins + TopMargins + 20;

        if (AutoScroll)
        {
            m_ContentPanel->ScrollToBottom();
        }
    }
}
