#pragma once
#include <utils/FileDialog.h>
#include <Cocoa/Cocoa.h>

namespace mc::utils {
class OSXFileDialog : public FileDialog {
public:
    // Inherited via FileDialog
    std::string chooseFolderDialog() override;
    std::string openFileDialog() override;
    std::string saveFileDialog() override;
};
} // namespace mc::utils
