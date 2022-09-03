#include "FileDialog.h"

namespace mc::utils {
    void FileDialogFilter::addFilter(const std::string& name, const std::string& formats) {
        d_filterMap.insert({ name, formats });
    }

    bool FileDialogFilter::hasFilters() {
        return static_cast<bool>(d_filterMap.size());
    }
} // namespace mc::utils
