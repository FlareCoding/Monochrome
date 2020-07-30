#pragma once
#include <core/Core.h>
#include <Windows.h>
#include <shobjidl.h>
#include <string>
#include <functional>
#include <map>

namespace mc
{
    /// Allows the user to easily create filters for the UIFileDialogue instance to use.
    class UIFileDialogueFilter
    {
    public:
        /// Returns the filter data in a format required by UIFileDialogue internal usage.
        std::vector<COMDLG_FILTERSPEC> GetComDlgFilterSpecs();

        /// Registers a filter for specific file format(s).
        /// @param name Describes the file formats being filtered.
        /// @param formats String of file formats separated by a semicolon, (i.e "*.png;*.jpg;*.jpeg;*.psd").
        void AddFilter(const std::wstring& name, const std::wstring& formats);

        /// @returns Tells whether any filters have been registered.
        bool HasFilters();

    private:
        std::map<std::wstring, std::wstring> m_FilterMap;
    };

    /// Opens a file explorer with a prompt to either select a file/directory or save a file.
    class UIFileDialogue
    {
    public:
        UIFileDialogue() = default;

        /// Opens a dialogue to select an existing directory.
        /// @returns String containing the path of the chosen directory, empty if nothing was selected
        std::string ChooseDirectoryDialogue();

        /// Opens a dialogue to select an existing file.
        /// @returns String containing the path of the chosen file, empty if nothing was selected
        std::string ChooseFileDialogue();

        /// Opens a dialogue to save a new file.
        /// @returns String containing the path to the new file, empty if nothing was selected
        std::string SaveFileDialogue();

        /// Sets a filter for file types to be shown.
        void SetFilter(UIFileDialogueFilter filter);

    private:
        IFileOpenDialog* m_pFileOpenDialogue;
        UIFileDialogueFilter m_Filter;

        std::string FireOpenFileDialogue(FILEOPENDIALOGOPTIONS options, bool open_dialogue);
    };
}
