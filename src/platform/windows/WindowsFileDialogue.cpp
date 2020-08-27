#include "WindowsFileDialogue.h"

namespace mc
{
	static std::vector<COMDLG_FILTERSPEC> GetComDlgFilterSpecs(UIFileDialogueFilter& filter)
	{
		std::vector<COMDLG_FILTERSPEC> filters;

		for (auto& pair : filter.GetFilters())
		{
			filters.push_back(
				{ pair.first.c_str(), pair.second.c_str() }
			);
		}

		return filters;
	}

	Ref<UIFileDialogue> UIFileDialogue::Create()
	{
		return Ref<UIFileDialogue>(new WindowsFileDialogue());
	}

	std::string WindowsFileDialogue::ChooseDirectoryDialogue()
	{
		return FireOpenFileDialogue(FOS_PICKFOLDERS | FOS_PATHMUSTEXIST, true);
	}

	std::string WindowsFileDialogue::ChooseFileDialogue()
	{
		return FireOpenFileDialogue(FOS_FILEMUSTEXIST | FOS_PATHMUSTEXIST, true);
	}

	std::string WindowsFileDialogue::SaveFileDialogue()
	{
		return FireOpenFileDialogue(FOS_PATHMUSTEXIST, false);
	}

	std::string WindowsFileDialogue::FireOpenFileDialogue(FILEOPENDIALOGOPTIONS options, bool open_dialogue)
	{
		std::string result = "";
		HRESULT hr = E_FAIL;

		if (open_dialogue)
			hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&m_pFileOpenDialogue));
		else
			hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL, IID_IFileSaveDialog, reinterpret_cast<void**>(&m_pFileOpenDialogue));

		if (!SUCCEEDED(hr))
			return result;

		if (m_Filter.HasFilters())
		{
			auto& filterspecs = GetComDlgFilterSpecs(m_Filter);
			m_pFileOpenDialogue->SetFileTypes((UINT)filterspecs.size(), &filterspecs[0]);
		}

		m_pFileOpenDialogue->SetOptions(options);
		hr = m_pFileOpenDialogue->Show(NULL);

		if (!SUCCEEDED(hr))
		{
			m_pFileOpenDialogue->Release();
			return result;
		}

		IShellItem* pItem;
		hr = m_pFileOpenDialogue->GetResult(&pItem);

		if (!SUCCEEDED(hr))
		{
			m_pFileOpenDialogue->Release();
			return result;
		}

		PWSTR pszFilePath;
		hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

		if (SUCCEEDED(hr))
		{
			size_t chars_converted = 0;
			char path[MAX_PATH];
			wcstombs_s(&chars_converted, path, pszFilePath, MAX_PATH);

			result = std::string(path);
			CoTaskMemFree(pszFilePath);
		}

		pItem->Release();
		m_pFileOpenDialogue->Release();
		return result;
	}
}
