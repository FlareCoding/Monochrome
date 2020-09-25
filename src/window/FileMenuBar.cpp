#include "FileMenuBar.h"

namespace mc
{
	FileMenuBar::FileMenuBar()
	{
	}

	FileMenuBar::~FileMenuBar()
	{
	}

	void FileMenuBar::AddMenu(const Ref<FileMenu>& menu)
	{
		Menus.push_back(menu);
	}

	void FileMenuBar::RemoveMenu(const Ref<FileMenu>& menu)
	{
		std::vector<Ref<FileMenu>>::iterator it = std::find(Menus.begin(), Menus.end(), menu);
		if (it != Menus.end())
			Menus.erase(it);
	}

	void FileMenuBar::Update()
	{
		for (Ref<FileMenu>& item : Menus)
			item->Update();
	}

	Ref<FileMenuBar> FileMenuBar::Create()
	{
		return Ref<FileMenuBar>(new FileMenuBar());
	}
}

