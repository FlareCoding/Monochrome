#include "UIDockPanel.h"
#include <graphics/Graphics.h>
#include <window/UIWindow.h>
#include <events/WindowEvents.h>
#include <events/CustomEvents.h>
#include <algorithm>

namespace mc
{
    template <typename T>
    bool CheckType(UIView* view)
    {
        return dynamic_cast<T*>(view);
    }

    UIDockPanel::UIDockPanel()
    {
        layer.frame = Frame(40, 40, 200, 200);
        SetDefaultOptions();
    }

    UIDockPanel::UIDockPanel(Frame frame)
    {
        layer.frame = frame;
        SetDefaultOptions();
    }
    
    void UIDockPanel::SetAnchoredToWindow(bool anchored)
    {
        if (!parent)
            m_AnchoredToWindow = anchored;
        else
            m_AnchoredToWindow = false;

        if (anchored && srcwindow)
            ResizeToWindow();
    }

    void UIDockPanel::SetDefaultOptions()
    {
        m_PreviousSize = layer.frame.size;
        SetupEventHandlers();
    }

    void UIDockPanel::SetupEventHandlers()
    {
        AddEventHandler<EventType::WindowResized>([this](Event& e, UIView* sender) -> bool {
            if (m_AnchoredToWindow && srcwindow)
                ResizeToWindow();

            return EVENT_UNHANDLED;
        });
    }

    void UIDockPanel::ResizeToWindow()
    {
        // May change in the future
        static float universal_border_height = (srcwindow->GetWindowStyle() == WindowStyle::Modern) ? 60.0f : 0.0f;

        layer.frame.position = Point{ 0, universal_border_height };
        layer.frame.size = Size{ (float)srcwindow->GetWidth(), (float)srcwindow->GetHeight() - universal_border_height };
    }

    void UIDockPanel::Draw()
    {
        Update();

        Graphics::DrawRectangle(
            layer.frame.position.x,
            layer.frame.position.y,
            layer.frame.size.width,
            layer.frame.size.height,
            layer.color,
            CornerRadius,
            Filled,
            Stroke
        );
    }
    
    void UIDockPanel::Update()
    {
        if (m_PreviousOwnerWindow != srcwindow)
        {
            m_PreviousOwnerWindow = srcwindow;

            if (m_AnchoredToWindow && srcwindow)
            {
                // Artificially create window resized event
                // so that the dock panel can properly resize.
                auto resize_event = std::make_shared<WindowResizedEvent>(
                    srcwindow->GetNativeHandle(),
                    srcwindow->GetWidth(),
                    srcwindow->GetHeight()
                );

                srcwindow->DispatchEvent(resize_event);
            }
        }

        if (m_PreviousSize.width != layer.frame.size.width ||
            m_PreviousSize.height != layer.frame.size.height)
        {
            m_PreviousSize = layer.frame.size;
            UpdateAnchoredViews();
        }
    }

    void UIDockPanel::OnSubviewAdded(const Ref<UIView>&)
    {
        UpdateAnchoredViews();
    }

    void UIDockPanel::OnSubviewRemoved(const Ref<UIView>&)
    {
        UpdateAnchoredViews();
    }

    void UIDockPanel::UpdateAnchoredViews()
    {
        // Sort the elements by anchor's priority so that even those 
        // that don't have an anchor will get Anchor::Center by default.
        std::sort(subviews.begin(), subviews.end(),
            [](const Ref<UIView>& lhs, const Ref<UIView>& rhs) -> bool {
                return lhs->anchor < rhs->anchor;
            }
        );

        Frame available_space_frame = Frame(0, 0, 0, 0);
        available_space_frame.size = layer.frame.size;

        for (auto& subview : subviews)
        {
            switch (subview->anchor)
            {
            case Anchor::Left:
            {
                subview->layer.frame.position.x = available_space_frame.position.x;
                subview->layer.frame.position.y = available_space_frame.position.y;
                subview->layer.frame.size.height = available_space_frame.size.height;

                // Adjusting the frame designated for available space that's left
                available_space_frame.position.x += subview->layer.frame.size.width;
                available_space_frame.size.width -= subview->layer.frame.size.width;
                break;
            }
            case Anchor::Right:
            {
                subview->layer.frame.position.x = available_space_frame.size.width - subview->layer.frame.size.width + available_space_frame.position.x;
                subview->layer.frame.position.y = available_space_frame.position.y;
                subview->layer.frame.size.height = available_space_frame.size.height;

                // Adjusting the frame designated for available space that's left
                available_space_frame.size.width -= subview->layer.frame.size.width;

                break;
            }
            case Anchor::Top:
            {
                subview->layer.frame.position.x = available_space_frame.position.x;
                subview->layer.frame.position.y = available_space_frame.position.y;
                subview->layer.frame.size.width = available_space_frame.size.width;

                // Adjusting the frame designated for available space that's left
                available_space_frame.position.y += subview->layer.frame.size.height;
                available_space_frame.size.height -= subview->layer.frame.size.height;
                break;
            }
            case Anchor::Bottom:
            {
                subview->layer.frame.position.x = available_space_frame.position.x;
                subview->layer.frame.position.y = available_space_frame.size.height - subview->layer.frame.size.height + available_space_frame.position.y;
                subview->layer.frame.size.width = available_space_frame.size.width;

                // Adjusting the frame designated for available space that's left
                available_space_frame.size.height -= subview->layer.frame.size.height;
                break;
            }
            case Anchor::Center:
            {
                subview->layer.frame.position.x = available_space_frame.position.x;
                subview->layer.frame.position.y = available_space_frame.position.y;
                subview->layer.frame.size.width = available_space_frame.size.width;
                subview->layer.frame.size.height = available_space_frame.size.height;

                // Adjusting the frame designated for available space that's left.
                // Center element will take up the entire space in the center.
                available_space_frame = Frame(0, 0, 0, 0);
                break;
            }
            default: break;
            }

            // Sanity checking the dimensions of the element
            if (subview->layer.frame.size.width < 0)
                subview->layer.frame.size.width = 0;

            if (subview->layer.frame.size.height < 0)
                subview->layer.frame.size.height = 0;

            // If the element is a dock panel as well, update its own docked views.
            if (CheckType<UIDockPanel>(subview.get()))
            {
                std::dynamic_pointer_cast<UIDockPanel>(subview)->UpdateAnchoredViews();
            }

            // Finally, dispatch a docking update event to notify the subviews.
            if (srcwindow)
            {
                auto e = MakeRef<DockingUpdateEvent>(this);
                srcwindow->DispatchEvent(e);
            }
        }
    }
}
