#include "ProgressBar.h"

namespace mc {
	ProgressBar::ProgressBar() {
		_createVisuals();
		_setupProperties();
	}

	Size ProgressBar::_measureSize() {
		return Size(fixedWidth, fixedHeight);
	}

	void ProgressBar::_createVisuals() {
		d_bodyVisual = MakeRef<RectVisual>();
		backgroundColor.forwardAssignment(&d_bodyVisual->color);
		addCoreVisualElement(d_bodyVisual);

		d_progressVisual = MakeRef<RectVisual>();
		progressColor.forwardAssignment(&d_progressVisual->color);
		addCoreVisualElement(d_progressVisual);
	}

	void ProgressBar::_setupProperties() {
		fixedHeight = 20;
		fixedWidth = 200;
		backgroundColor = Color::gray;
		progressColor = Color::red;
		backgroundColor.forwardEmittedEvents(this);
		progressColor.forwardEmittedEvents(this);
	}

}