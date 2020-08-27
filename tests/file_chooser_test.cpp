#include <Monochrome.h>
using namespace mc;

int main()
{
	auto window = UIWindow::Create(WindowStyle::Modern, 1000, 670, "File Chooser Test");
	window->SetBackgroundColor(Color(38, 38, 39, 1.0f));
	window->SetModernWindowButtonsColor(Color(38, 38, 39, 1.0f));

	Ref<UIButton> SelectDirectoryButton = MakeRef<UIButton>();
	SelectDirectoryButton->layer.frame = Frame({ 370, 120 }, { 260, 36 });
	SelectDirectoryButton->Label->Text = "Select Directory";
	SelectDirectoryButton->AddEventHandler<EventType::MouseButtonClicked>([window](Event& evt, UIView* sender) -> bool {
		UIFileDialogue fd;
		auto path = fd.ChooseDirectoryDialogue();
		if (!path.empty())
			printf("Chosen Path: %s\n", path.c_str());

		return EVENT_HANDLED;
	});
	window->AddView(SelectDirectoryButton);

	Ref<UIButton> SelectFileButton = MakeRef<UIButton>();
	SelectFileButton->layer.frame = Frame({ 370, 180 }, { 260, 36 });
	SelectFileButton->Label->Text = "Select File";
	SelectFileButton->AddEventHandler<EventType::MouseButtonClicked>([window](Event& evt, UIView* sender) -> bool {
		UIFileDialogue fd;
		auto path = fd.ChooseFileDialogue();
		if (!path.empty())
			printf("Chosen Path: %s\n", path.c_str());

		return EVENT_HANDLED;
	});
	window->AddView(SelectFileButton);

	Ref<UIButton> SaveFileButton = MakeRef<UIButton>();
	SaveFileButton->layer.frame = Frame({ 370, 240 }, { 260, 36 });
	SaveFileButton->Label->Text = "Save File";
	SaveFileButton->AddEventHandler<EventType::MouseButtonClicked>([window](Event& evt, UIView* sender) -> bool {
		UIFileDialogue fd;

		UIFileDialogueFilter filter;
		filter.AddFilter(L"All Files", L"*.*");
		filter.AddFilter(L"Text", L"*.txt;");
		filter.AddFilter(L"Image", L"*.png;*.jpg;*.jpeg");
		fd.SetFilter(filter);

		auto path = fd.SaveFileDialogue();
		if (!path.empty())
			printf("Chosen Path: %s\n", path.c_str());

		return EVENT_HANDLED;
	});
	window->AddView(SaveFileButton);

	window->StartWindowLoop();
}