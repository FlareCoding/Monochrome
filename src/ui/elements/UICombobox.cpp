#include "UICombobox.h"
#include <graphics/Graphics.h>
#include <events/Events.h>
#include <window/UIWindow.h>

namespace mc
{
	UICombobox::UICombobox()
	{
		layer.frame = Frame(40, 40, 180, 40);
		SetDefaultOptions();
	}

	UICombobox::UICombobox(Frame frame) : UIView(frame)
	{
		SetDefaultOptions();
	}

	void UICombobox::SetDefaultOptions()
	{
		SlotSize = layer.frame.size.height;
		CornerRadius = 4;

		m_DisplayItemLabel = MakeRef<UILabel>();
		m_DisplayItemLabel->Text = "";
		AddSubview(m_DisplayItemLabel);

		m_OpenSelectionArrowLabel = MakeRef<UILabel>();
		m_OpenSelectionArrowLabel->UseWidestringText = true;
		m_OpenSelectionArrowLabel->WidestringText = L"▼";
		AddSubview(m_OpenSelectionArrowLabel);

		m_ItemPanel = MakeRef<UIScrollPanel>(Frame(0, SlotSize, layer.frame.size.width, 0));
		AddSubview(m_ItemPanel);

		RecalculateItemPositions();
		SetupEventHandlers();
	}

	void UICombobox::SetupEventHandlers()
	{
		AddEventHandler<EventType::MouseButtonClicked>([this](Event& e, UIView* sender) -> bool {
			MouseButtonClickedEvent& mbce = reinterpret_cast<MouseButtonClickedEvent&>(e);
			if (mbce.button != MouseButton::Left)
				return EVENT_UNHANDLED;

			uint32_t dpi = 96;
			if (UIWindow::GetCurrentActiveWindow())
				dpi = UIWindow::GetCurrentActiveWindow()->GetDpi();

			Frame hittest_frame = m_DisplayItemLabel->layer.frame;
			hittest_frame.position = m_DisplayItemLabel->GetAbsolutePosition();
			hittest_frame.size.width += m_OpenSelectionArrowAreaWidth;
			hittest_frame.AdjustToDpi(dpi);

			if (hittest_frame.DoesContain(mbce.location))
			{
				m_ItemSelectionOpened = !m_ItemSelectionOpened;

				if (m_ItemSelectionOpened)
				{
					// Increase the Z-index to be at the top of all elements
					// to prevent graphics from being overlayed.
					m_UnopenedZIndex = GetZIndex();
					SetZIndex(GetHighestLayerZIndex() + 1);
				}
				else
					SetZIndex(m_UnopenedZIndex);

				RecalculateItemPositions();
			}

			return EVENT_UNHANDLED;
		});

		AddEventHandler<EventType::FocusChanged>([this](Event& e, UIView* sender) -> bool {
			FocusChangedEvent& fce = reinterpret_cast<FocusChangedEvent&>(e);
			if (!fce.GainedFocus)
			{
				m_ItemSelectionOpened = false;
				RecalculateItemPositions();
				SetZIndex(m_UnopenedZIndex);
			}

			return EVENT_UNHANDLED;
		});
	}

	void UICombobox::Draw()
	{
		Update();

		// Main area that displays the current element
		Graphics::DrawRectangle(
			layer.frame.position.x,
			layer.frame.position.y,
			layer.frame.size.width,
			SlotSize,
			layer.color,
			CornerRadius,
			true,
			0
		);
	}

	void UICombobox::Update()
	{
		// Keeping the label frame same as button's
		m_DisplayItemLabel->layer.frame = Frame(0, 0, layer.frame.size.width - m_OpenSelectionArrowAreaWidth, SlotSize);
		m_DisplayItemLabel->layer.color.alpha = layer.color.alpha;
		m_DisplayItemLabel->color.alpha = layer.color.alpha;

		m_OpenSelectionArrowLabel->layer.frame = Frame(layer.frame.size.width - m_OpenSelectionArrowAreaWidth, 0, m_OpenSelectionArrowAreaWidth, SlotSize);
		m_OpenSelectionArrowLabel->layer.color.alpha = layer.color.alpha;
		m_OpenSelectionArrowLabel->color.alpha = layer.color.alpha;

		m_ItemPanel->layer.color.alpha = layer.color.alpha;
		m_ItemPanel->Visible = m_ItemSelectionOpened;
		m_ItemPanel->ContentView->layer.color = m_ItemBackgroundColor;
	}

	void UICombobox::AddItem(const std::string& item)
	{
		InsertItem(item, m_Items.size());
	}

	void UICombobox::InsertItem(const std::string& item, size_t index)
	{
		if (index > m_Items.size()) return;

		auto ItemButton = MakeRef<UIButton>();
		ItemButton->Label->Text = item;
		ItemButton->CornerRadius = 0;
		ItemButton->layer.color = m_ItemBackgroundColor;
		ItemButton->Label->color = m_ItemTextColor;
		ItemButton->AddEventHandler<EventType::MouseButtonClicked>([this, item](Event& e, UIView* sender) -> bool {
			MouseButtonClickedEvent& mbce = reinterpret_cast<MouseButtonClickedEvent&>(e);
			if (mbce.button != MouseButton::Left)
				return EVENT_UNHANDLED;
			
			// This check has to be done since the scroll panel
			// will receive events regardless of whether or not
			// the scroll panel is opened.
			if (m_ItemSelectionOpened && m_ItemPanel->Visible)
			{
				m_DisplayItemLabel->Text = item;
				m_ItemSelectionOpened = false;

				RecalculateItemPositions();
				SetZIndex(m_UnopenedZIndex);

				size_t item_idx = IndexOf(item);
				if (item_idx != m_SelectedIndex)
				{
					m_SelectedIndex = item_idx;
					CallItemChangedEventHandlers();
				}

				return EVENT_HANDLED;
			}

			return EVENT_UNHANDLED;
		});

		m_Items.push_back({ item, ItemButton });

		RecalculateItemPositions();

		if (m_Items.size() == 1)
		{
			m_DisplayItemLabel->Text = item;
			m_SelectedIndex = 0;
			CallItemChangedEventHandlers();
		}
	}

	void UICombobox::AddItems(const std::vector<std::string>& items)
	{
		for (auto& item : items)
			AddItem(item);
	}

	void UICombobox::SetItems(const std::vector<std::string>& items)
	{
		Clear();
		AddItems(items);
	}

	size_t UICombobox::IndexOf(const std::string& item)
	{
		for (size_t i = 0; i < m_Items.size(); i++)
			if (m_Items[i].first == item)
				return i;

		return -1;
	}

	std::string UICombobox::GetItem(size_t index)
	{
		if (index >= m_Items.size())
			return "";
		else
			return m_Items[index].first;
	}

	void UICombobox::RemoveItem(size_t index)
	{
		if (index >= m_Items.size()) return;

		auto& button = m_Items[index].second;
		m_Items.erase(m_Items.begin() + index);
		m_ItemPanel->RemoveChild(button);

		RecalculateItemPositions();

		size_t display_item_index = IndexOf(m_DisplayItemLabel->Text);
		if (display_item_index > m_Items.size())
		{
			if (!m_Items.size())
				m_DisplayItemLabel->Text = "";
			else
			{
				m_DisplayItemLabel->Text = GetItem(0);
				m_SelectedIndex = 0;
				CallItemChangedEventHandlers();
			}
		}
	}

	void UICombobox::RemoveItem(const std::string& item)
	{
		RemoveItem(IndexOf(item));
	}

	void UICombobox::Clear()
	{
		for (auto it = m_Items.begin(); it != m_Items.end();)
		{
			m_ItemPanel->RemoveChild(it->second);
			it = m_Items.erase(it);
		}

		m_SelectedIndex = -1;
		m_DisplayItemLabel->Text = "";
		RecalculateItemPositions();
	}

	void UICombobox::SelectItem(size_t index)
	{
		if (index > m_Items.size() || !m_Items.size()) return;

		if (index != m_SelectedIndex)
		{
			m_DisplayItemLabel->Text = GetItem(index);
			m_ItemSelectionOpened = false;
			m_SelectedIndex = index;
			CallItemChangedEventHandlers();
		}
	}

	void UICombobox::SelectItem(const std::string& item)
	{
		SelectItem(IndexOf(item));
	}

	size_t UICombobox::GetItemCount()
	{
		return m_Items.size();
	}

	std::string UICombobox::GetSelectedItem()
	{
		return GetItem(m_SelectedIndex);
	}

	void UICombobox::RecalculateItemPositions()
	{
		layer.frame.size.height = SlotSize;

		Position ItemPosition = { 0, 0 };
		m_ItemPanel->Clear();

		for (auto& [item_name, button] : m_Items)
		{
			button->layer.frame = Frame(ItemPosition, Size{ layer.frame.size.width - 10.0f, SlotSize }); // - 10.0f comes from the width of the scrollbar
			m_ItemPanel->AddChild(button);

			ItemPosition.y += SlotSize;

			if (m_ItemSelectionOpened)
				layer.frame.size.height += SlotSize;
		}

		m_ItemPanel->ContentView->layer.frame.size.height = ItemPosition.y;

		if (ItemPosition.y < layer.frame.size.height - SlotSize)
			m_ItemPanel->layer.frame.size.height = ItemPosition.y;
		else
			m_ItemPanel->layer.frame.size.height = layer.frame.size.height - SlotSize;
	}

	void UICombobox::CallItemChangedEventHandlers()
	{
		for (auto& cb : m_ItemChangedCallbacks)
			cb(m_SelectedIndex, this);
	}

	void UICombobox::SetItemBackgroundColor(Color color)
	{
		m_ItemBackgroundColor = color;

		for (auto& [item_name, button] : m_Items)
			button->layer.color = m_ItemBackgroundColor;
	}

	Color UICombobox::GetItemBackgroundColor()
	{
		return m_ItemBackgroundColor;
	}

	void UICombobox::SetItemTextColor(Color color)
	{
		m_ItemTextColor = color;

		for (auto& [item_name, button] : m_Items)
			button->Label->color = m_ItemTextColor;
	}

	Color UICombobox::GetItemTextColor()
	{
		return m_ItemTextColor;
	}

	void UICombobox::SetDropdownArrowColor(Color color)
	{
		m_OpenSelectionArrowLabel->color = color;
	}

	void UICombobox::SetSelectedItemColor(Color color)
	{
		m_DisplayItemLabel->color = color;
	}
}
