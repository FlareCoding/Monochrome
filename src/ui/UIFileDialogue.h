#pragma once
#include <core/Core.h>
#include <string>
#include <functional>
#include <map>

#if defined(_WIN32)
    #include <Windows.h>
    #include <shobjidl.h>
#endif

namespace mc
{
    /// Allows the user to easily create filters for the UIFileDialogue instance to use.
    class UIFileDialogueFilter
    {
    public:
        /// Registers a filter for specific file format(s).
        /// @param name Describes the file formats being filtered.
        /// @param formats String of file formats separated by a semicolon, (i.e "*.png;*.jpg;*.jpeg;*.psd").
        virtual void AddFilter(const std::wstring& name, const std::wstring& formats);

        /// @returns Tells whether any filters have been registered.
        bool HasFilters();

        /// Returns the list of all filters in a map form.
        std::map<std::wstring, std::wstring>& GetFilters() { return m_FilterMap; };

    private:
        std::map<std::wstring, std::wstring> m_FilterMap;
    };

    /// Opens a file explorer with a prompt to either select a file/directory or save a file.
    class UIFileDialogue
    {
    public:
        static Ref<UIFileDialogue> Create();

        virtual ~UIFileDialogue() = default;

        /// Opens a dialogue to select an existing directory.
        /// @returns String containing the path of the chosen directory, empty if nothing was selected
        virtual std::string ChooseDirectoryDialogue() = 0;

        /// Opens a dialogue to select an existing file.
        /// @returns String containing the path of the chosen file, empty if nothing was selected
        virtual std::string ChooseFileDialogue() = 0;

        /// Opens a dialogue to save a new file.
        /// @returns String containing the path to the new file, empty if nothing was selected
        virtual std::string SaveFileDialogue() = 0;

        /// Sets a filter for file types to be shown.
        void SetFilter(UIFileDialogueFilter filter) { m_Filter = filter; };

    protected:
        UIFileDialogueFilter m_Filter;
    };
}
