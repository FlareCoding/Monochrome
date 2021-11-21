#include "OSXWindow.h"
#include <graphics/Graphics.h>
#include <ui/UIElements.h>
#include <events/Events.h>

#include "OSXNativeCore.h"
#include <unistd.h>

namespace mc
{
    static OSXWindow* s_CurrentActiveWindowInstance = nullptr;

    UIWindow* UIWindow::GetCurrentActiveWindow()
    {
        return s_CurrentActiveWindowInstance;
    }

    Ref<UIWindow> UIWindow::Create(WindowStyle style, uint32_t width, uint32_t height, const char* title)
    {
        return Ref<UIWindow>(new OSXWindow(style, width, height, title));
    }

    Ref<UIWindow> UIWindow::CreateOverlayWindow(const char* targetWindow, const char* title)
	{
		return nullptr;
	}

    OSXWindow::OSXWindow(WindowStyle style, uint32_t width, uint32_t height, const char* title)
    {
        m_WindowStyle = WindowStyle::Classic; // modern style is not supported yet
        m_Width = width;
        m_Height = height;
        m_Title = title;

        m_NativeObject = MakeRef<OSXCocoaNativeObject>();
        Init();
    }

    void OSXWindow::Init()
    {
        // Application
        m_NativeObject->application = [NSApplication sharedApplication];
        [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
        
        // Window
        m_NativeObject->window = [[NSWindow alloc] initWithContentRect:NSMakeRect(100, 300, m_Width, m_Height)
                                                            styleMask:NSWindowStyleMaskTitled
                                | NSWindowStyleMaskClosable
                                | NSWindowStyleMaskMiniaturizable
                                | NSWindowStyleMaskResizable
                                                            backing:NSBackingStoreBuffered defer:NO];
        
        
        m_NativeObject->windowDelegate = [OSXWindowDelegate alloc];
        [m_NativeObject->window setDelegate:(id)m_NativeObject->windowDelegate];
        m_NativeObject->windowDelegate.mcWindowHandle = this;

        [m_NativeObject->window setAcceptsMouseMovedEvents:YES];
        [m_NativeObject->window setTitle:[NSString stringWithUTF8String: m_Title]];
        [m_NativeObject->window makeKeyAndOrderFront:nil];
        [NSApp activateIgnoringOtherApps:YES];
        
        // Content View
        m_NativeObject->contentView = [[OSXWindowContentViewDelegate alloc] initWithFrame:NSMakeRect(100.0, 300.0, m_Width, m_Height)];
        ((OSXWindowContentViewDelegate*)m_NativeObject->contentView).mcWindowHandle = this;
        [m_NativeObject->window setContentView:m_NativeObject->contentView];
        
        // Sets the refresh rate of the content view
        [NSTimer scheduledTimerWithTimeInterval:1.0/60.0 target:m_NativeObject->contentView selector:@selector(update) userInfo:nil repeats:YES];

        Graphics::Initialize(m_NativeObject.get());
        Graphics::SetActiveTarget(m_NativeObject.get());

        s_CurrentActiveWindowInstance = this;
        m_IsOpened = true;
    }

    void OSXWindow::SetupModernWindowViews()
    {
    }

    void OSXWindow::AdjustModernWindowViews()
    {
    }

    void OSXWindow::ForceUpdate(bool clear_screen)
    {

    }

    void OSXWindow::Update()
    {
        m_InternalTimer.reset();
        m_SceneManager.ProcessEvents(m_Dpi);
        auto EventProcessingTime = m_InternalTimer.elapsed();

        m_InternalTimer.reset();
        Graphics::Update(m_Background, m_SceneManager);
        auto GraphicsRenderingTime = m_InternalTimer.elapsed();

        auto e = std::make_shared<WindowUpdatedEvent>(nullptr, EventProcessingTime, GraphicsRenderingTime);
        m_SceneManager.DispatchEvent(e);
        m_SceneManager.ProcessEvents(m_Dpi);

		if (HasMenuBar)
		{
			MenuBar->Update();
		}
    }

    void OSXWindow::StartWindowLoop()
    {
        [NSApp run];
    }

    void OSXWindow::DispatchEvent(EventPtr event)
    {
        m_SceneManager.DispatchEvent(event);
    }

    void OSXWindow::ProcessImmediateEvents()
    {
        m_SceneManager.ProcessEvents(m_Dpi);
    }

    void OSXWindow::SetSize(uint32_t width, uint32_t height)
    {
        NSRect frame = [m_NativeObject->window frame];
        frame.origin.y -= frame.size.height;
        frame.origin.y += height;

        frame.size.width = width;
        frame.size.height = height;

        [m_NativeObject->window setFrame:frame display:YES animate:NO];
    }

    void OSXWindow::SetPos(uint32_t x, uint32_t y)
    {
        NSRect frame = [m_NativeObject->window frame];
        frame.origin.x = x;
        frame.origin.y = y;
        [m_NativeObject->window setFrame:frame display:YES animate:NO];
    }

    void OSXWindow::SetTitle(const char* title)
    {
        [m_NativeObject->window setTitle:[NSString stringWithUTF8String:title]];
    }

    void OSXWindow::AddView(Ref<UIView> view)
    {
        view->srcwindow = this;
        m_SceneManager.AddView(view);
    }

    void OSXWindow::RemoveView(Ref<UIView> view)
    {
        m_SceneManager.RemoveView(view);
    }

    void OSXWindow::SetModernWindowButtonsColor(Color color)
    {
        if (m_ModernWindowCloseButton)
            m_ModernWindowCloseButton->layer.color = color;

        if (m_ModernWindowMaximizeButton)
            m_ModernWindowMaximizeButton->layer.color = color;

        if (m_ModernWindowMinimizeButton)
            m_ModernWindowMinimizeButton->layer.color = color;
    }

    void OSXWindow::FocusView(Ref<UIView> view)
    {
        if (m_FocusedView == view.get()) return;

        // If a view was already focused, unfocus it and call event handlers
        if (m_FocusedView)
        {
            for (auto& callback : m_FocusedView->GetEventHandlers(EventType::FocusChanged))
            {
                auto event = FocusChangedEvent(false);
                callback(event, m_FocusedView);
            }
        }

        // Set the new focused view
        m_FocusedView = view.get();

        // If there is a new focused view, call its event handlers
        if (m_FocusedView)
        {
            for (auto& callback : m_FocusedView->GetEventHandlers(EventType::FocusChanged))
            {
                auto event = FocusChangedEvent(true);
                callback(event, m_FocusedView);
            }
        }
    }

    Ref<UIView> OSXWindow::GetViewRef(UIView* raw_address)
    {
        return m_SceneManager.GetViewRef(raw_address);
    }

    Position OSXWindow::GetMouseCursorPos()
    {
        return { 0, 0 };
    }

    Position OSXWindow::GetAsboluteMouseCursorPos()
    {
        return { 0, 0 };
    }

    void OSXWindow::RemoveAllViews()
    {
        for (auto& view : m_SceneManager.GetViewsList())
        {
            if (view.get() == m_ModernWindowDragPanel.get()) continue;
            if (view.get() == m_ModernWindowCloseButton.get()) continue;
            if (view.get() == m_ModernWindowMaximizeButton.get()) continue;
            if (view.get() == m_ModernWindowMinimizeButton.get()) continue;
            if (view.get() == m_ModernWindowTitleLabel.get()) continue;

            RemoveView(view);
        }
    }

    void OSXWindow::SetMenuBar(const Ref<FileMenuBar>& menuBar)
	{
		MenuBar = menuBar;
		HasMenuBar = true;
	}
	
	const std::pair<float, float> OSXWindow::GetLastViewPosition()
	{
		auto &view = m_SceneManager.GetViewsList().back();
		if (view == nullptr)
			return { 0.0f, 0.0f };

		return { view->layer.frame.position.x, view->layer.frame.position.y };
	}
}
