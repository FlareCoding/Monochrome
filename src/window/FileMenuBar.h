#pragma once

#include "core/Core.h"
#include "window/FileMenu.h"

namespace mc
{
	class FileMenuBar
	{
	private:
		std::vector<Ref<FileMenu>> Menus;

	public:
		FileMenuBar();
		~FileMenuBar();

		void AddMenu(const Ref<FileMenu>& menu);
		void RemoveMenu(const Ref<FileMenu>& menu);
		void Update();

		const std::vector<Ref<FileMenu>>& GetMenus() const { return Menus; }

		static Ref<FileMenuBar> Create();
	};
}