#include "UIScrollPanel.h"
#include <graphics/Graphics.h>
#include <events/Events.h>

namespace mc
{
	UIScrollPanel::UIScrollPanel()
	{
		layer.frame = Frame(100, 100, 200, 400);
		SetDefaultOptions();
	}

	UIScrollPanel::UIScrollPanel(Frame frame) : UIView(frame)
	{
		SetDefaultOptions();
	}

	void UIScrollPanel::SetDefaultOptions()
	{
		ContentView = MakeRef<UIView>();
		ContentView->layer.color = Color::transparent;
		ContentView->layer.frame = Frame(0, 0, layer.frame.size.width, layer.frame.size.height * 1.2f);
		AddSubview(ContentView);

		m_VerticalScrollbar = MakeRef<UIButton>();
		m_VerticalScrollbar->layer.color = ScrollbarColor;
		m_VerticalScrollbar->layer.frame = Frame(layer.frame.size.width - m_ScrollbarWidth, 0, m_ScrollbarWidth, layer.frame.size.height * 0.9f);
		m_VerticalScrollbar->CornerRadius = 4;
		m_VerticalScrollbar->Label->Text.clear();
		m_VerticalScrollbar->SetZIndex(10000);
		AddSubview(m_VerticalScrollbar);

		SetupEventHandlers();
	}

	void UIScrollPanel::SetupEventHandlers()
	{
		m_VerticalScrollbar->AddEventHandler<EventType::MouseButtonPressed>([this](Event& e, UIView* sender) -> bool {
			MouseButtonPressedEvent& evt = reinterpret_cast<MouseButtonPressedEvent&>(e);
			if (evt.button == MouseButton::Left)
				m_VerticalScrollbarMousePressed = true;

			return EVENT_UNHANDLED;
		});

		AddEventHandler<EventType::MouseButtonReleased>([this](Event& e, UIView* sender) -> bool {
			MouseButtonReleasedEvent& evt = reinterpret_cast<MouseButtonReleasedEvent&>(e);
			if (evt.button == MouseButton::Left)
				m_VerticalScrollbarMousePressed = false;

			return EVENT_UNHANDLED;
		});

		AddEventHandler<EventType::MouseMoved>([this](Event& e, UIView* sender) -> bool {
			MouseMovedEvent& evt = reinterpret_cast<MouseMovedEvent&>(e);
			if (evt.button == MouseButton::Left && m_VerticalScrollbarMousePressed)
				ScrollContent(evt.distance.y * 1.6f);

			return EVENT_UNHANDLED;
		});

		AddEventHandler<EventType::MouseHoverOff>([this](Event& e, UIView* sender) -> bool {
			m_VerticalScrollbarMousePressed = false;
			return EVENT_UNHANDLED;
		});

		m_VerticalScrollbar->AddEventHandler<EventType::MouseScrolled>([this](Event& e, UIView* sender) -> bool {
			MouseScrolledEvent& evt = reinterpret_cast<MouseScrolledEvent&>(e);	
			ScrollContent(-evt.ScrollDistance / 2.0f);

			return EVENT_UNHANDLED;
		});
	}

	void UIScrollPanel::ScrollContent(float distance)
	{
		ContentView->layer.frame.position.y -= distance;

		if (ContentView->layer.frame.position.y > 0)
			ContentView->layer.frame.position.y = 0;

		float scroll = GetVerticalScrollAmount();
		float max_scroll = GetMaximumVerticalScrollAmount();

		if (scroll > max_scroll)
			ContentView->layer.frame.position.y += (scroll - max_scroll);
	}

	void UIScrollPanel::Draw()
	{
		Update();

		// Main content area
		Graphics::DrawRectangle(
			layer.frame.position.x, 
			layer.frame.position.y, 
			layer.frame.size.width, 
			layer.frame.size.height, 
			layer.color, 
			CornerRadius, 
			true
		);
	}
	
	void UIScrollPanel::Update()
	{
		// Keeping horizontal size and position constraints constant
		ContentView->layer.frame.position.x = 0;
		ContentView->layer.frame.size.width = layer.frame.size.width;

		if (ContentView->layer.frame.position.y > 0)
			ContentView->layer.frame.position.y = 0;

		ContentView->CornerRadius = CornerRadius;

		// Scrollbar
		ProcessScrollbar();
	}

	float UIScrollPanel::GetMaximumVerticalScrollAmount()
	{
		float orig = ContentView->layer.frame.position.y;
		ContentView->layer.frame.position.y = 0;
		
		float space = (ContentView->GetAbsolutePosition().y + ContentView->layer.frame.size.height) - (GetAbsolutePosition().y + layer.frame.size.height);
		ContentView->layer.frame.position.y = orig;

		return (space > 0) ? space : 0;
	}

	float UIScrollPanel::GetVerticalScrollAmount()
	{
		float current_scroll = (ContentView->GetAbsolutePosition().y + ContentView->layer.frame.size.height) - (GetAbsolutePosition().y + layer.frame.size.height);
		return GetMaximumVerticalScrollAmount() - current_scroll;
	}

	void UIScrollPanel::ProcessScrollbar()
	{
		m_VerticalScrollbar->layer.color = ScrollbarColor;

		m_VerticalScrollbar->layer.frame.position.x = layer.frame.size.width - m_ScrollbarWidth;

		if (m_VerticalScrollbar->layer.frame.position.y < 0)
			m_VerticalScrollbar->layer.frame.position.y = 0;

		if (m_VerticalScrollbar->layer.frame.position.y + m_VerticalScrollbar->layer.frame.size.height > layer.frame.size.height)
			m_VerticalScrollbar->layer.frame.position.y = layer.frame.size.height - m_VerticalScrollbar->layer.frame.size.height;

		if (layer.frame.size.height > ContentView->layer.frame.size.height)
		{
			m_VerticalScrollbar->layer.frame.size.height = 0;
			return;
		}
		else
		{
			float scrollbar_height = layer.frame.size.height / ContentView->layer.frame.size.height;
			m_VerticalScrollbar->layer.frame.size.height = scrollbar_height * layer.frame.size.height;
		}

		float percentage_scrolled = GetVerticalScrollAmount() / GetMaximumVerticalScrollAmount();
		float gap = layer.frame.size.height - m_VerticalScrollbar->layer.frame.size.height;

		m_VerticalScrollbar->layer.frame.position.y = percentage_scrolled * gap;
	}

	void UIScrollPanel::AddChild(const Ref<UIView>& view)
	{
		ContentView->AddSubview(view);
	}
}
