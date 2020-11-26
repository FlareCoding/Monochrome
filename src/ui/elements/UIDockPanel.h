#pragma once
#include <ui/UIView.h>

namespace mc
{
	class UIDockPanel : public UIView
	{
	public:
		UIDockPanel();
		UIDockPanel(Frame frame);
		~UIDockPanel() = default;

		// Inherited via IDrawable
		virtual void Draw() override;

		bool	Filled = true;
		float	Stroke = 2;

		/// Recalculates the positions of anchored elements.
		void UpdateAnchoredViews();

		/// Determines whether or not the dock panel is bound to the entire window's dimensions.
		/// 
		/// If set to true, the dock panel will take up the entire window's width
		/// and height and will resize along with the window when its dimensions change.
		/// 
		/// @note This will only work if the dock panel doesn't have any parent view.
		void SetAnchoredToWindow(bool anchored);

	private:
		void SetDefaultOptions();
		void SetupEventHandlers();
		void Update();

		// Tracks the previous size of the dock panel
		Size m_PreviousSize = { 0, 0 };

		// Tracks the previous owner source window
		UIWindow* m_PreviousOwnerWindow = nullptr;

		// Indicates whether or not the dock panel itself
		// should be dynamically scaled relative to the window.
		bool m_AnchoredToWindow = false;

		// Resized the dock panel to be the same size as the window
		void ResizeToWindow();

	protected:
		virtual void OnSubviewAdded(const Ref<UIView>&) override;
		virtual void OnSubviewRemoved(const Ref<UIView>&) override;
	};
}
