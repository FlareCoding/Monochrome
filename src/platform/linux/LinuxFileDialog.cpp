#include "LinuxFileDialog.h"
#include <utils/TextUtils.h>
#include <vector>

namespace mc::utils {
    Shared<FileDialog> FileDialog::create() {
        return Shared<FileDialog>(new LinuxFileDialog());
    }

    std::string LinuxFileDialog::chooseFolderDialog() {
        return "";
    }

    std::string LinuxFileDialog::openFileDialog() {
        return "";
    }

    std::string LinuxFileDialog::saveFileDialog() {
        return "";
    }
} // namespace mc::utils
