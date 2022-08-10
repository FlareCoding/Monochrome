#pragma once
#include "Label.h"
#include "visuals/RectVisual.h"
#include "visuals/BorderVisual.h"

namespace mc {
	class ProgressBar : public BaseWidget {
	public:
		ProgressBar();

		PropertyObserver<Color>       backgroundColor;
		PropertyObserver<Color>       progressColor;

	protected:
		Size _measureSize() override;

	private:
		void _createVisuals();
		void _setupProperties();

		Shared<RectVisual> d_bodyVisual;
		Shared<RectVisual> d_progressVisual;
	};
}