#pragma once
#include <ui/UIFileDialogue.h>

namespace mc
{
    class OSXFileDialogue : public UIFileDialogue
    {
    public:
        // Inherited via UIFileDialogue
        virtual std::string ChooseDirectoryDialogue() override;
        virtual std::string ChooseFileDialogue() override;
        virtual std::string SaveFileDialogue() override;
    };
}
