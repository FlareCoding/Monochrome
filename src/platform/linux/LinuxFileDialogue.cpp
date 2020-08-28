#include "LinuxFileDialogue.h"

namespace mc
{
    Ref<UIFileDialogue> UIFileDialogue::Create()
	{
		return Ref<UIFileDialogue>(new LinuxFileDialogue());
	}

	std::string LinuxFileDialogue::ChooseDirectoryDialogue()
	{
		char filename[1024];
		FILE *f = popen("zenity --file-selection --directory 2> /dev/null", "r");
		fgets(filename, 1024, f);
		return std::string(filename);
	}

	std::string LinuxFileDialogue::ChooseFileDialogue()
	{
		char filename[1024];
		FILE *f = popen("zenity --file-selection 2> /dev/null", "r");
		fgets(filename, 1024, f);
		return std::string(filename);
	}

	std::string LinuxFileDialogue::SaveFileDialogue()
	{
		char filename[1024];
		FILE *f = popen("zenity --file-selection --save 2> /dev/null", "r");
		fgets(filename, 1024, f);
		return std::string(filename);
	}
}
