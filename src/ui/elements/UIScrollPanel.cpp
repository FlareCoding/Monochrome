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
		m_VerticalScrollbar->layer.frame = Frame(layer.frame.size.width - m_ScrollbarThickness, 0, m_ScrollbarThickness, layer.frame.size.height * 0.9f);
		m_VerticalScrollbar->CornerRadius = 4;
		m_VerticalScrollbar->Label->Text.clear();
		m_VerticalScrollbar->SetZIndex(10000);
		AddSubview(m_VerticalScrollbar);

		m_HorizontalScrollbar = MakeRef<UIButton>();
		m_HorizontalScrollbar->layer.color = ScrollbarColor;
		m_HorizontalScrollbar->layer.frame = Frame(0, layer.frame.size.height - m_ScrollbarThickness, layer.frame.size.width, m_ScrollbarThickness);
		m_HorizontalScrollbar->CornerRadius = 4;
		m_HorizontalScrollbar->Label->Text.clear();
		m_HorizontalScrollbar->SetZIndex(10000);
		AddSubview(m_HorizontalScrollbar);

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

		m_HorizontalScrollbar->AddEventHandler<EventType::MouseButtonPressed>([this](Event& e, UIView* sender) -> bool {
			MouseButtonPressedEvent& evt = reinterpret_cast<MouseButtonPressedEvent&>(e);
			if (evt.button == MouseButton::Left)
				m_HorizontalScrollbarMousePressed = true;

			return EVENT_UNHANDLED;
		});

		AddEventHandler<EventType::MouseButtonReleased>([this](Event& e, UIView* sender) -> bool {
			MouseButtonReleasedEvent& evt = reinterpret_cast<MouseButtonReleasedEvent&>(e);
			if (evt.button == MouseButton::Left)
			{
				m_VerticalScrollbarMousePressed = false;
				m_HorizontalScrollbarMousePressed = false;
			}

			return EVENT_UNHANDLED;
		});

		AddEventHandler<EventType::MouseMoved>([this](Event& e, UIView* sender) -> bool {
			MouseMovedEvent& evt = reinterpret_cast<MouseMovedEvent&>(e);
			if (evt.button == MouseButton::Left && m_IsFocused)
			{
				if (m_VerticalScrollbarMousePressed)
					ScrollContent(evt.distance.y * 1.6f);

				if (m_HorizontalScrollbarMousePressed)
					ScrollContentHorizontally(evt.distance.x * 1.6f);
			}

			return EVENT_UNHANDLED;
		});

		AddEventHandler<EventType::MouseHoverOff>([this](Event& e, UIView* sender) -> bool {
			m_VerticalScrollbarMousePressed = false;
			m_HorizontalScrollbarMousePressed = false;
			return EVENT_UNHANDLED;
		});

		m_VerticalScrollbar->AddEventHandler<EventType::MouseScrolled>([this](Event& e, UIView* sender) -> bool {
			MouseScrolledEvent& evt = reinterpret_cast<MouseScrolledEvent&>(e);	
			
			if (m_IsFocused)
				ScrollContent(-evt.ScrollDistance / 2.0f);

			return EVENT_UNHANDLED;
		});

		AddEventHandler<EventType::MouseHoverOn>([this](Event& e, UIView* sender) -> bool {
			m_IsFocused = true;
			return EVENT_UNHANDLED;
		});

		AddEventHandler<EventType::MouseHoverOff>([this](Event& e, UIView* sender) -> bool {
			m_IsFocused = false;
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

	void UIScrollPanel::ScrollContentHorizontally(float distance)
	{
		ContentView->layer.frame.position.x -= distance;

		if (ContentView->layer.frame.position.x > 0)
			ContentView->layer.frame.position.x = 0;

		float scroll = GetHorizontalScrollAmount();
		float max_scroll = GetMaximumHorizontalScrollAmount();

		if (scroll > max_scroll)
			ContentView->layer.frame.position.x += (scroll - max_scroll);
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
		// Keeping the srcwindow field updated
		ContentView->srcwindow = srcwindow;

		if (ContentView->layer.frame.position.x > 0)
			ContentView->layer.frame.position.x = 0;

		if (ContentView->layer.frame.position.y > 0)
			ContentView->layer.frame.position.y = 0;

		ContentView->CornerRadius = CornerRadius;

		// Scrollbar
		ProcessScrollbar();
	}

	void UIScrollPanel::ScrollToTop()
	{
		ScrollContent(-100000000.0f);
	}

	void UIScrollPanel::ScrollToBottom()
	{
		ScrollContent(100000000.0f);
	}

	void UIScrollPanel::ScrollToRight()
	{
		ScrollContentHorizontally(100000000.0f);
	}

	void UIScrollPanel::ScrollToLeft()
	{
		ScrollContentHorizontally(-100000000.0f);
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

	float UIScrollPanel::GetMaximumHorizontalScrollAmount()
	{
		float orig = ContentView->layer.frame.position.x;
		ContentView->layer.frame.position.x = 0;

		float space = (ContentView->GetAbsolutePosition().x + ContentView->layer.frame.size.width) - (GetAbsolutePosition().x + layer.frame.size.width);
		ContentView->layer.frame.position.x = orig;

		return (space > 0) ? space : 0;
	}

	float UIScrollPanel::GetHorizontalScrollAmount()
	{
		float current_scroll = (ContentView->GetAbsolutePosition().x + ContentView->layer.frame.size.width) - (GetAbsolutePosition().x + layer.frame.size.width);
		return GetMaximumHorizontalScrollAmount() - current_scroll;
	}

	void UIScrollPanel::ProcessScrollbar()
	{
		m_VerticalScrollbar->layer.color = ScrollbarColor;
		m_HorizontalScrollbar->layer.color = ScrollbarColor;

		// Keep the scrollbars at correct coordinates
		m_VerticalScrollbar->layer.frame.position.x = layer.frame.size.width - m_ScrollbarThickness;
		m_HorizontalScrollbar->layer.frame.position.y = layer.frame.size.height - m_ScrollbarThickness;

		// Sanitize and clamp vertical scrollbar coordinates
		if (m_VerticalScrollbar->layer.frame.position.y < 0)
			m_VerticalScrollbar->layer.frame.position.y = 0;

		if (m_VerticalScrollbar->layer.frame.position.y + m_VerticalScrollbar->layer.frame.size.height > layer.frame.size.height)
			m_VerticalScrollbar->layer.frame.position.y = layer.frame.size.height - m_VerticalScrollbar->layer.frame.size.height;

		// Sanitize and clamp horizontal scrollbar coordinates
		if (m_HorizontalScrollbar->layer.frame.position.x < 0)
			m_HorizontalScrollbar->layer.frame.position.x = 0;

		if (m_HorizontalScrollbar->layer.frame.position.x + m_HorizontalScrollbar->layer.frame.size.width > layer.frame.size.width)
			m_HorizontalScrollbar->layer.frame.position.x = layer.frame.size.width - m_HorizontalScrollbar->layer.frame.size.width;

		// If either scrollbar is not needed (if content view fits perfectly into the panel),
		// then set the size of it to 0 to hide it.
		//
		// Vertical Scrollbar
		if (layer.frame.size.height > ContentView->layer.frame.size.height)
		{
			m_VerticalScrollbar->layer.frame.size.height = 0;
		}
		else
		{
			float scrollbar_height = layer.frame.size.height / ContentView->layer.frame.size.height;
			m_VerticalScrollbar->layer.frame.size.height = scrollbar_height * layer.frame.size.height;
		}

		//
		// Horizontal Scrollbar
		if (layer.frame.size.width > ContentView->layer.frame.size.width)
		{
			m_HorizontalScrollbar->layer.frame.size.width = 0;
		}
		else
		{
			float scrollbar_width = layer.frame.size.width / ContentView->layer.frame.size.width;
			m_HorizontalScrollbar->layer.frame.size.width = scrollbar_width * layer.frame.size.width;
		}

		//
		// Calculating scrollbar offsets
		//
		// Vertical
		float percentage_scrolled_vertical = GetVerticalScrollAmount() / GetMaximumVerticalScrollAmount();
		float vertical_gap = layer.frame.size.height - m_VerticalScrollbar->layer.frame.size.height;

		m_VerticalScrollbar->layer.frame.position.y = percentage_scrolled_vertical * vertical_gap;

		//
		// Horizontal
		float percentage_scrolled_horizontal = GetHorizontalScrollAmount() / GetMaximumHorizontalScrollAmount();
		float horizontal_gap = layer.frame.size.width - m_HorizontalScrollbar->layer.frame.size.width;

		m_HorizontalScrollbar->layer.frame.position.x = percentage_scrolled_horizontal * horizontal_gap;
	}

	void UIScrollPanel::AddChild(const Ref<UIView>& view)
	{
		ContentView->AddSubview(view);
	}

	void UIScrollPanel::RemoveChild(Ref<UIView> view)
	{
		ContentView->RemoveSubview(view);
	}

	void UIScrollPanel::Clear()
	{
		ContentView->subviews.clear();
	}
}
