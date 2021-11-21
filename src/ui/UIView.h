#pragma once
#include <core/Core.h>
#include "IDrawable.h"
#include "Layer.h"
#include "Anchor.h"
#include "UICursor.h"
#include <events/IResponder.h>

namespace mc
{
	class UIWindow;

	class UIView : public IDrawable, public IResponder
	{
	public:
		// Inherited via IDrawable
		virtual void Draw() override;

	public:
		UIView() = default;
		virtual ~UIView() = default;

		UIView(const Ref<UIView>& view);
		UIView(Frame frame);

		/// Sets the heirarchical position of the view.
		void SetZIndex(uint32_t index);

		/// @returns Heirarchical position of the view.
		inline const uint32_t GetZIndex() const { return m_zIndex; }

		/// Default view's rounded corner radius.
		float CornerRadius = 6;

	public:
		/// Contains position, size, and color of the UIView.
		Layer layer = Layer();

		/// @returns Position relative to the top left corner of the UIWindow it is in.
		Position GetAbsolutePosition();

	public:
		/// List of children views.
		std::vector<Ref<UIView>> subviews;

		/// Pointer to the parent view, nullptr if no parent view exists.
		UIView* parent = nullptr;

		/// Pointer to the source window, initially a nullptr.
		UIWindow* srcwindow = nullptr;

		/// Cursor icon to be used when hovered over the view.
		CursorType cursor = CursorType::Arrow;

		/// Refers to the side of the parent this view should be docked to.
		/// @note This only gets applied when parent view is of type UIDockPanel.
		Anchor anchor = Anchor::None;

		/// Adds a child subview.
		/// Children subviews have position relative to their parent view's position.
		template <typename T>
		void AddSubview(Ref<T> subview)
		{
			subview->parent = this;
			subview->srcwindow = srcwindow;
			subviews.push_back(std::dynamic_pointer_cast<UIView>(subview));
			SortElements();
			OnSubviewAdded(subview);
		}

		/// Removes a child view.
		template <typename T>
		void RemoveSubview(Ref<T> subview)
		{
			std::vector<Ref<UIView>>::iterator it = std::find(subviews.begin(), subviews.end(), subview);
			if (it != subviews.end())
			{
				(*it)->parent = nullptr;
				subviews.erase(it);

				OnSubviewRemoved(subview);
			}

			SortElements();
		}

	private:
		friend class EventValidator;

	private:
		/// Tells whether or not mouse cursor is hovered over the view's frame.
		bool m_IsMouseOverFrame = false;

		/// Determines heirarchical position of the view in the list.
		uint32_t m_zIndex = 0;

		/// Sorts all subviews by their zIndex in increasing order.
		void SortElements();

	protected:
		virtual void OnSubviewAdded(const Ref<UIView>&) {}
		virtual void OnSubviewRemoved(const Ref<UIView>&) {}
	};

#define CastToUiView(elem) std::dynamic_pointer_cast<UIView>(elem)
}
