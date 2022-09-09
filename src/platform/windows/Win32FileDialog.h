#pragma once
#include <utils/FileDialog.h>
#include <Windows.h>
#include <shobjidl.h>

#define MAX_SPEC_SIZE 151
#define MAX_SPEC_BUFFERS 12

namespace mc::utils {
class Win32FileDialog : public FileDialog {
public:
    // Inherited via FileDialog
    std::string chooseFolderDialog() override;
    std::string openFileDialog() override;
    std::string saveFileDialog() override;

private:
    struct FilterSpecBuffer {
        wchar_t name[MAX_SPEC_SIZE];
        wchar_t spec[MAX_SPEC_SIZE];
    };

    IFileOpenDialog* d_pFileOpenDialogue = nullptr;
    std::string _fireOpenFileDialogue(FILEOPENDIALOGOPTIONS options, bool openDialogue);

    FilterSpecBuffer d_filterSpecBuffer[MAX_SPEC_BUFFERS];
    std::vector<COMDLG_FILTERSPEC> _getComDlgFilterSpecs(FileDialogFilter& filter);
};
} // namespace mc::utils
