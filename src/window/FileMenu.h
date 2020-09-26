#pragma once

#include <string>
#include <vector>
#include <functional>

#include "core/Core.h"
#include "ui/elements/UIButton.h"

namespace mc
{
	using FileCallbackFn = std::function<void()>;
	struct FileMenuItem;

	class FileMenu
	{
	private:
		std::vector<FileMenuItem> MenuItems;
		std::vector<Ref<UIButton>> MenuButtons;
		std::string MenuName;
		Ref<UIButton> NameButton;

		void ReCreateButtons();

	public:
		FileMenu(const std::string& menuName);
		~FileMenu();

		void AddMenuItem(const Ref<FileMenuItem>& item);
		void AddMenuItem(const FileMenuItem& item);
		void AddMenuItem(const std::string& name, const FileCallbackFn& fn, bool choosable = true);
		void AddSubMenu(const Ref<FileMenu>& other);
		void AddMenuSeperator();

		void Update();

		const std::string &GetName() const { return MenuName; }
		const std::vector<FileMenuItem>& GetMenuItems() const { return MenuItems; }

		bool operator==(const FileMenu& other);
		bool operator==(const FileMenu& other) const;
		bool operator!=(const FileMenu& other);
		bool operator!=(const FileMenu& other) const;

		static Ref<FileMenu> Create(const std::string& menuName);
	};

	struct FileMenuItem
	{
		std::string Name = "DefaultMenuItem";
		bool Choosable = true;
		bool Seperator = false;
		bool IsSubMenu = false;
		Ref<FileMenu> SubMenu = nullptr;
		FileCallbackFn CallbackFn;

		FileMenuItem() = default;
		FileMenuItem(const FileMenuItem&) = default;
		FileMenuItem &operator=(const FileMenuItem&) = default;
	};


}

