#pragma once
#include "UIButton.h"

namespace mc
{
	class UITabView : public UIView
	{
	public:
		UITabView();
		UITabView(Frame frame);
		~UITabView() = default;

		// Inherited via IDrawable
		virtual void Draw() override;

		float TabAreaHeight = 24.0f;
		float TabWidth = 140.0f;
		bool UnderlineTabs = false;
		Color TabUnderlineColor = Color::white;
		Color SelectedTabColor = Color::gray;

		/// Adds a tab to the top bar and reserves a UIView for that tab.
		/// @param name String containing the name for the tab.
		/// @returns Reference to the newly created UIView reserved for the tab.
		Ref<UIView> AddTab(const std::string& name);

		/// Removes a tab at specified index from the top bar.
		void RemoveTab(size_t index);

		/// Removes a tab with specified name from the top bar.
		void RemoveTab(const std::string& name);

		/// Returns the UIView reserved for the tab at the specified index.
		/// Can return nullptr if tab at specified index doesn't exist.
		Ref<UIView> GetTab(size_t index);

		/// Returns the UIView reserved for the tab with the specified name.
		/// Can return nullptr if tab with specified name doesn't exist.
		Ref<UIView> GetTab(const std::string& name);

		/// Sets active the UIView reserved for the tab at the specified index.
		void OpenTab(size_t index);

		/// Sets active the UIView reserved for the tab with the specified name.
		void OpenTab(const std::string& name);

		/// Returns the currently opened tab name.
		/// Can return an empty string if no tab is opened.
		std::string GetOpenedTab();

		/// Returns the number of added tabs.
		size_t GetTabCount();

		/// Returns the UIView reserved for the tab at the specified index.
		/// Can return nullptr if tab at specified index doesn't exist.
		Ref<UIView> operator[](size_t index);

		/// Returns the UIView reserved for the tab with the specified name.
		/// Can return nullptr if tab with specified name doesn't exist.
		Ref<UIView> operator[](const std::string& name);

		/// Iterates over every tab and performes a given styling function on the tab's button.
		/// @param fn User defined function that performs the styling on every tab button.
		void StyleTabButtons(std::function<void(Ref<UIButton>& tab)> fn);

	private:
		void SetDefaultOptions();
		void Update();
	
	private:
		// ========== Variables Effecting Dimensions ==========
		float m_OldTabAreaHeight = 24.0f;
		float m_OldTabWidth = 24.0f;

		// ============ TABS ============
		Ref<UIView> m_TabsAreaView;
		std::vector<std::tuple<std::string, Ref<UIButton>, Ref<UIView>>> m_Tabs;
		Ref<UIView> m_SelectedView = nullptr;
		Ref<UIView> m_CurrentlyPlacedView = nullptr;
		Color m_ChosenTabColor = Color::transparent;

		void UpdateTabButtonsColors();
		void RecalculateTabButtonsPositions();
	};
}
