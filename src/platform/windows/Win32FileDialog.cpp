#include "Win32FileDialog.h"
#include <utils/TextUtils.h>
#include <vector>

namespace mc::utils {
    static std::vector<COMDLG_FILTERSPEC> GetComDlgFilterSpecs(FileDialogFilter& filter) {
        std::vector<COMDLG_FILTERSPEC> filters;

        for (auto& [name, spec] : filter.getFilters()) {
            auto nameWideStr = utils::convertToWideString(name);
            auto specWideStr = utils::convertToWideString(spec);

            COMDLG_FILTERSPEC filterSpec;
            filterSpec.pszName = nameWideStr.c_str();
            filterSpec.pszSpec = specWideStr.c_str();

            filters.push_back(filterSpec);
        }

        return filters;
    }

    Shared<FileDialog> FileDialog::create() {
        return Shared<FileDialog>(new Win32FileDialog());
    }

    std::string Win32FileDialog::chooseFolderDialog() {
        return fireOpenFileDialogue(FOS_PICKFOLDERS | FOS_PATHMUSTEXIST, true);
    }

    std::string Win32FileDialog::openFileDialog() {
        return fireOpenFileDialogue(FOS_FILEMUSTEXIST | FOS_PATHMUSTEXIST, true);
    }

    std::string Win32FileDialog::saveFileDialog() {
        return fireOpenFileDialogue(FOS_PATHMUSTEXIST, false);
    }

    std::string Win32FileDialog::fireOpenFileDialogue(
        FILEOPENDIALOGOPTIONS options,
        bool openDialogue
    ) {
        std::string result = "";
        HRESULT hr = E_FAIL;

        if (openDialogue) {
            hr = CoCreateInstance(
                CLSID_FileOpenDialog,
                NULL, CLSCTX_ALL,
                IID_IFileOpenDialog,
                reinterpret_cast<void**>(&d_pFileOpenDialogue)
            );
        } else {
            hr = CoCreateInstance(
                CLSID_FileSaveDialog,
                NULL,
                CLSCTX_ALL,
                IID_IFileSaveDialog,
                reinterpret_cast<void**>(&d_pFileOpenDialogue)
            );
        }

        if (!SUCCEEDED(hr)) {
            return result;
        }

        if (d_filter.hasFilters()) {
            auto filterspecs = GetComDlgFilterSpecs(d_filter);
            d_pFileOpenDialogue->SetFileTypes((UINT)filterspecs.size(), &filterspecs[0]);
        }

        d_pFileOpenDialogue->SetOptions(options);

        if (!d_defaultPath.empty()) {
            IShellItem* customFolderLocation;
            hr = SHCreateItemFromParsingName(
                utils::convertToWideString(d_defaultPath).c_str(),
                nullptr,
                IID_PPV_ARGS(&customFolderLocation)
            );

            if (!SUCCEEDED(hr)) {
                d_pFileOpenDialogue->Release();
                return result;
            }
        }

        hr = d_pFileOpenDialogue->Show(NULL);

        if (!SUCCEEDED(hr)) {
            d_pFileOpenDialogue->Release();
            return result;
        }

        IShellItem* pItem;
        hr = d_pFileOpenDialogue->GetResult(&pItem);

        if (!SUCCEEDED(hr)) {
            d_pFileOpenDialogue->Release();
            return result;
        }

        PWSTR pszFilePath;
        hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

        if (SUCCEEDED(hr)) {
            size_t chars_converted = 0;
            char path[MAX_PATH] = { 0 };
            wcstombs_s(&chars_converted, path, pszFilePath, MAX_PATH - 1);

            result = std::string(path);
            CoTaskMemFree(pszFilePath);
        }

        pItem->Release();
        d_pFileOpenDialogue->Release();
        return result;
    }
} // namespace mc::utils
