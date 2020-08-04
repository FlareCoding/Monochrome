#include "UITabView.h"
#include <graphics/Graphics.h>
#include <events/Events.h>

namespace mc
{
	UITabView::UITabView()
	{
		layer.frame = Frame(40, 40, 380, 480);
		SetDefaultOptions();
	}
	
	UITabView::UITabView(Frame frame) : UIView(frame)
	{
		SetDefaultOptions();
	}

	void UITabView::SetDefaultOptions()
	{
		layer.color = Color::transparent;
		CornerRadius = 0;

		m_TabsAreaView = MakeRef<UIView>();
		m_TabsAreaView->CornerRadius = 0;
		m_TabsAreaView->layer.color = Color::transparent;
		AddSubview(m_TabsAreaView);
	}

	void UITabView::Draw()
	{
		Update();

		Graphics::DrawRectangle(
			layer.frame.position.x,
			layer.frame.position.y,
			layer.frame.size.width,
			layer.frame.size.height,
			layer.color,
			CornerRadius,
			true,
			0.0f
		);

		if (UnderlineTabs)
		{
			Graphics::DrawLine(
				layer.frame.position.x,
				layer.frame.position.y + TabAreaHeight + 2,
				layer.frame.position.x + TabWidth * m_Tabs.size(),
				layer.frame.position.y + TabAreaHeight + 2,
				TabUnderlineColor,
				2.0f
			);
		}
	}

	void UITabView::Update()
	{
		m_TabsAreaView->layer.frame = Frame(0, 0, layer.frame.size.width, TabAreaHeight);

		if (m_CurrentlyPlacedView != m_SelectedView)
		{
			if (m_CurrentlyPlacedView) RemoveSubview(m_CurrentlyPlacedView);
			m_CurrentlyPlacedView = m_SelectedView;
			
			if (m_CurrentlyPlacedView) AddSubview(m_CurrentlyPlacedView);

			for (auto [name, tab, view] : m_Tabs)
				UpdateSourceWindowProperty(view);
		}

		float TabViewOffsetY = UnderlineTabs ? 3.0f : 0.0f;
		if (m_CurrentlyPlacedView) m_CurrentlyPlacedView->layer.frame = Frame(0, TabAreaHeight + TabViewOffsetY, layer.frame.size.width, layer.frame.size.height - TabAreaHeight - TabViewOffsetY);

		if (m_OldTabAreaHeight != TabAreaHeight)
		{
			RecalculateTabButtonsPositions();
			m_OldTabAreaHeight = TabAreaHeight;
		}
		if (m_OldTabWidth != TabWidth)
		{
			RecalculateTabButtonsPositions();
			m_OldTabWidth = TabWidth;
		}

		for (auto [name, tab_btn, view] : m_Tabs)
			if (m_CurrentlyPlacedView == view)
				tab_btn->layer.color = SelectedTabColor;
	}

	void UITabView::UpdateTabButtonsColors()
	{
		for (auto [name, tab_btn, view] : m_Tabs)
			if (m_SelectedView != view)
				tab_btn->layer.color = m_ChosenTabColor;
			else
				tab_btn->layer.color = SelectedTabColor;
	}

	void UITabView::RecalculateTabButtonsPositions()
	{
		Position button_position = { 0, 0 };
		for (auto& [name, button, view] : m_Tabs)
		{
			button->layer.frame = Frame(button_position.x, button_position.y, TabWidth, TabAreaHeight);
			button_position += { TabWidth, 0 };

			view->layer.frame = Frame(0, TabAreaHeight, layer.frame.size.width, layer.frame.size.height - TabAreaHeight);
			view->CornerRadius = CornerRadius;
		}

		UpdateTabButtonsColors();
	}

	void UITabView::UpdateSourceWindowProperty(Ref<UIView> view)
	{
		view->srcwindow = srcwindow;
		for (auto& subview : view->subviews)
			UpdateSourceWindowProperty(subview);
	}

	Ref<UIView> UITabView::AddTab(const std::string& name)
	{
		Ref<UIView> tab_view = MakeRef<UIView>();

		Ref<UIButton> tab_btn = MakeRef<UIButton>();
		tab_btn->CornerRadius = 0;
		tab_btn->Label->Text = name;
		
		if (m_ChosenTabColor != Color::transparent)
			tab_btn->layer.color = m_ChosenTabColor;
		else
			m_ChosenTabColor = tab_btn->layer.color;

		tab_btn->AddEventHandler<EventType::MouseButtonClicked>([this, tab_btn](Event& e, UIView* sender) -> bool {
			MouseButtonClickedEvent& mbce = reinterpret_cast<MouseButtonClickedEvent&>(e);
			if (mbce.button == MouseButton::Left)
				OpenTab(tab_btn->Label->Text);

			return EVENT_HANDLED;
		});
		m_TabsAreaView->AddSubview(tab_btn);
		m_Tabs.push_back({ name, tab_btn, tab_view });

		RecalculateTabButtonsPositions();

		return tab_view;
	}

	void UITabView::RemoveTab(size_t index)
	{
		if (!m_Tabs.size() || index < 0 || index > m_Tabs.size()) return;

		auto [name, tab_btn, view] = m_Tabs[index];
		m_Tabs.erase(m_Tabs.begin() + index);

		if (m_CurrentlyPlacedView == view)
			m_SelectedView = nullptr;

		m_TabsAreaView->RemoveSubview(tab_btn);
		RecalculateTabButtonsPositions();
	}

	void UITabView::RemoveTab(const std::string& name)
	{
		for (size_t i = 0; i < m_Tabs.size(); i++)
			if (std::get<0>(m_Tabs[i])._Equal(name))
				RemoveTab(i);
	}

	Ref<UIView> UITabView::GetTab(size_t index)
	{
		return operator[](index);
	}

	Ref<UIView> UITabView::GetTab(const std::string& name)
	{
		return operator[](name);
	}

	void UITabView::OpenTab(size_t index)
	{
		if (!m_Tabs.size() || index < 0 || index > m_Tabs.size()) return;

		auto [name, button, view] = m_Tabs[index];
		m_SelectedView = view;

		UpdateTabButtonsColors();
	}

	void UITabView::OpenTab(const std::string& name)
	{
		for (size_t i = 0; i < m_Tabs.size(); i++)
			if (std::get<0>(m_Tabs[i])._Equal(name))
				return OpenTab(i);
	}

	std::string UITabView::GetOpenedTab()
	{
		if (!m_CurrentlyPlacedView) return "";

		for (auto [name, tab_btn, view] : m_Tabs)
			if (view.get() == m_CurrentlyPlacedView.get())
				return name;

		return "";
	}

	size_t UITabView::GetTabCount()
	{
		return m_Tabs.size();
	}

	Ref<UIView> UITabView::operator[](size_t index)
	{
		if (!m_Tabs.size() || index < 0 || index > m_Tabs.size()) return nullptr;

		auto [name, button, view] = m_Tabs[index];
		return view;
	}

	Ref<UIView> UITabView::operator[](const std::string& name)
	{
		for (size_t i = 0; i < m_Tabs.size(); i++)
			if (std::get<0>(m_Tabs[i])._Equal(name))
				return std::get<2>(m_Tabs[i]);

		return nullptr;
	}

	void UITabView::StyleTabButtons(std::function<void(Ref<UIButton>& tab)> fn)
	{
		for (auto& [name, tab_btn, view] : m_Tabs)
		{
			fn(tab_btn);
			m_ChosenTabColor = tab_btn->layer.color;
		}
	}
}
