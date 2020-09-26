#include "FileMenu.h"

#include "window/UIWindow.h"

namespace mc
{
	void FileMenu::ReCreateButtons()
	{
		MenuButtons.clear();
		MenuButtons.shrink_to_fit();

		float xOffset = 0.0f;
		float yOffset = 0.0f;
		for (FileMenuItem& item : MenuItems)
		{
			auto fileMenuBtn = MakeRef<UIButton>(Frame(xOffset, yOffset, 120, 25));
			fileMenuBtn->Filled = false;
			fileMenuBtn->Stroke = 0;
			fileMenuBtn->layer.color = Color::green;
			fileMenuBtn->Label->color = Color::white;
			fileMenuBtn->Label->Text = item.Name;

			fileMenuBtn->AddEventHandler<EventType::MouseButtonClicked>([&](Event& e, UIView* sender) -> bool
			{
				if (item.CallbackFn)
					item.CallbackFn();

				return EVENT_HANDLED;
			});

			xOffset += 120.0f;
			yOffset += 30.0f;
			MenuButtons.push_back(fileMenuBtn);
		}
	}

	FileMenu::FileMenu(const std::string& menuName)
		: MenuName(menuName)
	{
		// Set MenuButton
		NameButton = MakeRef<UIButton>(Frame(50, 10, 120, 25));
		NameButton->Filled = false;
		NameButton->Stroke = 0;
		NameButton->layer.color = Color::green;
		NameButton->Label->color = Color::white;
		NameButton->Label->Text = MenuName;

		NameButton->AddEventHandler<EventType::MouseButtonClicked>([&](Event& e, UIView* sender) -> bool
		{
			// Open Popup Menu

			return EVENT_HANDLED;
		});

		UIWindow::GetCurrentActiveWindow()->AddView(NameButton);
	}

	FileMenu::~FileMenu()
	{
	}

	void FileMenu::AddMenuItem(const Ref<FileMenuItem>& item)
	{
		MenuItems.push_back(*item);
		ReCreateButtons();
	}

	void FileMenu::AddMenuItem(const FileMenuItem& item)
	{
		MenuItems.push_back(item);
		ReCreateButtons();
	}

	void FileMenu::AddMenuItem(const std::string& name, const FileCallbackFn& fn, bool choosable)
	{
		FileMenuItem item;
		item.Name = name;
		item.Choosable = choosable;
		item.Seperator = false;
		item.IsSubMenu = false;
		item.CallbackFn = fn;
		MenuItems.push_back(item);
		ReCreateButtons();
	}

	void FileMenu::AddSubMenu(const Ref<FileMenu>& other)
	{
		FileMenuItem item;
		item.IsSubMenu = true;
		item.Seperator = false;
		item.Choosable = true;
		item.SubMenu = other;
		MenuItems.push_back(item);
		ReCreateButtons();
	}
	
	void FileMenu::AddMenuSeperator()
	{
		FileMenuItem item;
		item.Choosable = false;
		item.Seperator = true;
		MenuItems.push_back(item);
		ReCreateButtons();
	}
	
	void FileMenu::Update()
	{
	}
	
	bool FileMenu::operator==(const FileMenu& other)
	{
		if (MenuName == other.GetName())
			return true;
		else
			return false;
	}

	bool FileMenu::operator==(const FileMenu& other) const
	{
		if (MenuName == other.GetName())
			return true;
		else
			return false;
	}

	bool FileMenu::operator!=(const FileMenu& other)
	{
		return !(*this == other);
	}

	bool FileMenu::operator!=(const FileMenu& other) const
	{
		return !(*this == other);
	}

	Ref<FileMenu> FileMenu::Create(const std::string& menuName)
	{
		return Ref<FileMenu>(new FileMenu(menuName));
	}


}

