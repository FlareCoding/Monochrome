#pragma once
#include <core/Core.h>
#include <string>
#include <vector>
#include <map>

namespace mc::utils {
// Allows the user to easily create filters for the FileDialog instance to use.
class FileDialogFilter {
public:
    // Registers a filter for specific file format(s).
    // @param name Describes the file formats being filtered.
    // @param formats String of file formats separated
    // by a semicolon, (i.e "*.png;*.jpg;*.jpeg;*.psd").
    virtual void addFilter(const std::string& name, const std::vector<std::string>& formats);

    // @returns Tells whether any filters have been registered.
    bool hasFilters();

    // Returns the list of all filters in a map form.
    std::map<std::string, std::vector<std::string>>& getFilters() {
        return d_filterMap;
    }

private:
    std::map<std::string, std::vector<std::string>> d_filterMap;
};

class FileDialog {
public:
    static Shared<FileDialog> create();
    virtual ~FileDialog() = default;

    // Opens a dialogue to select an existing directory.
    // @returns String containing the path of the chosen directory, empty if nothing was selected
    virtual std::string chooseFolderDialog() = 0;

    // Opens a dialogue to select an existing file.
    // @returns String containing the path of the chosen file, empty if nothing was selected
    virtual std::string openFileDialog() = 0;

    // Opens a dialogue to save a new file.
    // @returns String containing the path to the new file, empty if nothing was selected
    virtual std::string saveFileDialog() = 0;

    // Sets a filter for file types to be shown.
    inline void setFilter(FileDialogFilter filter) {
        d_filter = filter;
    }

    // Sets the default location of the file dialog
    inline void setDefaultPath(const std::string& path) {
        d_defaultPath = path;
    }

protected:
    FileDialogFilter d_filter;
    std::string d_defaultPath;
};
} // namespace mc::utils
