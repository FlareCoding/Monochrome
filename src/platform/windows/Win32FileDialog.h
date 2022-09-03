#pragma once
#include <utils/FileDialog.h>
#include <Windows.h>
#include <shobjidl.h>

namespace mc::utils {
class Win32FileDialog : public FileDialog {
public:
    // Inherited via FileDialog
    std::string chooseFolderDialog() override;
    std::string openFileDialog() override;
    std::string saveFileDialog() override;

private:
    IFileOpenDialog* d_pFileOpenDialogue = nullptr;
    std::string fireOpenFileDialogue(FILEOPENDIALOGOPTIONS options, bool openDialogue);
};
} // namespace mc::utils
