#pragma once
#include <utils/FileDialog.h>

#define MAX_SPEC_SIZE 151
#define MAX_SPEC_BUFFERS 12

namespace mc::utils {
class LinuxFileDialog : public FileDialog {
public:
    // Inherited via FileDialog
    std::string chooseFolderDialog() override;
    std::string openFileDialog() override;
    std::string saveFileDialog() override;

private:
};
} // namespace mc::utils
