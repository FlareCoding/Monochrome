#pragma once
#include "UIScrollPanel.h"

namespace mc
{
	class UICombobox : public UIView
	{
	public:
		UICombobox();
		UICombobox(Frame frame);
		~UICombobox() = default;

		// Inherited via IDrawable
		virtual void Draw() override;

		/// Size of each slot and the main element displaying area.
		float SlotSize = 20.0f;

		/// Appends an item slot to the end of the selection view with a new string item.
		void AddItem(const std::string& item);

		/// Adds an item slot at a specified index of the selection view with a new string item.
		void InsertItem(const std::string& item, size_t index);

		/// Adds all the items in the vector to the combobox item list.
		void AddItems(const std::vector<std::string>& items);

		/// Removes all existing items in the list and adds the specified ones.
		void SetItems(const std::vector<std::string>& items);

		/// Returns the index of the item with specified text.
		/// If no item with such text is found, returns -1.
		size_t IndexOf(const std::string& item);

		/// Returns the item at the given index.
		/// If index is invalid (out of range), returns an empty string.
		std::string GetItem(size_t index);

		/// Removes an item at a specified index.
		void RemoveItem(size_t index);

		/// Searches and removes an item based on the specified name.
		void RemoveItem(const std::string& item);

		/// Removes all items from the combobox.
		void Clear();

		/// Sets the currently selected item to the item at a specified index.
		void SelectItem(size_t index);

		/// Sets the currently selected item to the specified item.
		void SelectItem(const std::string& item);

		/// Returns the number of items in the combobox.
		size_t GetItemCount();

		/// Returns currently selected item.
		std::string GetSelectedItem();

		/// Sets the background color of items in the selection list.
		void SetItemBackgroundColor(Color color);

		/// Returns the background color of items in the selection list.
		Color GetItemBackgroundColor();

		/// Returns the color of the dropdown arrow.
		Color GetDropdownArrowColor();

		/// Sets the text color of items in the selection list.
		void SetItemTextColor(Color color);

		/// Returns the text color of items in the selection list.
		Color GetItemTextColor();

		/// Sets the color of the dropdown arrow.
		void SetDropdownArrowColor(Color color);

		/// Sets the color of the selected item being displayed.
		void SetSelectedItemColor(Color color);

	public:
		using ComboboxItemChangedCallbackFn = std::function<void(size_t index, UICombobox* sender)>;

		/// Adds a callback function that gets called every time the selected item of the combobox changes.
		void AddItemChangedEventHandler(ComboboxItemChangedCallbackFn cb) { m_ItemChangedCallbacks.push_back(cb); }

		/// Adds a callback function that gets called every time the selected item of the combobox changes.
		void operator +=(ComboboxItemChangedCallbackFn cb) { m_ItemChangedCallbacks.push_back(cb); }

	private:
		void SetDefaultOptions();
		void SetupEventHandlers();

	private:
		void Update();

		// Z-index before unrolling the item panel
		uint32_t m_UnopenedZIndex = 1;

		// Displays the currently selected item
		Ref<UILabel> m_DisplayItemLabel;

		// Displays the downward arrow representing the open selection button
		Ref<UILabel> m_OpenSelectionArrowLabel;

		float m_OpenSelectionArrowAreaWidth = 20.0f;

		Color m_ItemBackgroundColor = Color::white;
		Color m_ItemTextColor = Color::black;

	private:
		bool m_ItemSelectionOpened = false;
		Ref<UIScrollPanel> m_ItemPanel;

	private:
		// Holds the items used in the combobox
		std::vector<std::pair<std::string, Ref<UIButton>>> m_Items;

		size_t m_SelectedIndex = -1;

		void RecalculateItemPositions();

	private:
		std::vector<ComboboxItemChangedCallbackFn> m_ItemChangedCallbacks;

		void CallItemChangedEventHandlers();
	};
}
