#include "UIFileDialogue.h"

namespace mc
{
	void UIFileDialogueFilter::AddFilter(const std::wstring& name, const std::wstring& formats)
	{
		m_FilterMap.insert({name, formats});
	}

	bool UIFileDialogueFilter::HasFilters()
	{
		return m_FilterMap.size();
	}
}
