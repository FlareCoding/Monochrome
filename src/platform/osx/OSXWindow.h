#pragma once
#include <window/UIWindow.h>
#include <chrono>

struct OSXCocoaNativeObject;

namespace mc
{
	class OSXWindow : public UIWindow
	{
	public:
		// Inherited via UIWindow
		virtual void SetSize(uint32_t width, uint32_t height) override;
		virtual void SetPos(uint32_t x, uint32_t y) override;
		virtual void SetTitle(const char* title) override;
		virtual void AddView(Ref<UIView> view) override;
		virtual void RemoveView(Ref<UIView> view) override;
		virtual void Update() override;
		virtual void ForceUpdate(bool clear_screen = false) override;
		virtual void StartWindowLoop() override;
		virtual void SetModernWindowButtonsColor(Color color) override;
		virtual void FocusView(Ref<UIView> view) override;
		virtual Ref<UIView> GetViewRef(UIView* raw_address) override;
		virtual Position GetMouseCursorPos() override;
		virtual Position GetAsboluteMouseCursorPos() override;
		virtual void RemoveAllViews() override;
		virtual void SetMenuBar(const Ref<FileMenuBar> &menuBar) override;
		virtual const std::pair<float, float> GetLastViewPosition() override;

		void DispatchEvent(EventPtr event);
		void ProcessImmediateEvents();

	private:
		Ref<OSXCocoaNativeObject> m_NativeObject = nullptr;

	private:
		virtual void Init() override;
		virtual void SetupModernWindowViews() override;
		virtual void AdjustModernWindowViews() override;

		void ProcessEvents(void* evt);

	public:
		OSXWindow(WindowStyle style, uint32_t width, uint32_t height, const char* title);

	private:
		class InternalTimer
		{
		public:
			InternalTimer() : beg_(clock_::now()) {}
			void reset() { beg_ = clock_::now(); }
			double elapsed() const {
				return std::chrono::duration_cast<second_>
					(clock_::now() - beg_).count();
			}

		private:
			typedef std::chrono::high_resolution_clock clock_;
			typedef std::chrono::duration<double, std::ratio<1> > second_;
			std::chrono::time_point<clock_> beg_;
		};

		InternalTimer m_InternalTimer;
		Ref<FileMenuBar> MenuBar = nullptr;
		bool HasMenuBar = false;
	};
}
