#pragma once
#include <ui/UIFileDialogue.h>

namespace mc
{
    class WindowsFileDialogue : public UIFileDialogue
    {
    public:
        // Inherited via UIFileDialogue
        virtual std::string ChooseDirectoryDialogue() override;
        virtual std::string ChooseFileDialogue() override;
        virtual std::string SaveFileDialogue() override;

    private:
        IFileOpenDialog* m_pFileOpenDialogue = nullptr;
        std::string FireOpenFileDialogue(FILEOPENDIALOGOPTIONS options, bool open_dialogue);
    };
}
