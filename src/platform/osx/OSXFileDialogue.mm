#include "OSXFileDialogue.h"

namespace mc
{
    Ref<UIFileDialogue> UIFileDialogue::Create()
	{
		return Ref<UIFileDialogue>(new OSXFileDialogue());
	}

	std::string OSXFileDialogue::ChooseDirectoryDialogue()
	{
		return "";
	}

	std::string OSXFileDialogue::ChooseFileDialogue()
	{
		return "";
	}

	std::string OSXFileDialogue::SaveFileDialogue()
	{
		return "";
	}
}
