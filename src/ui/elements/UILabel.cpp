#include "UILabel.h"
#include <graphics/Graphics.h>

namespace mc
{
	UILabel::UILabel(Frame frame)
		: UIView(frame)
	{
	}

	void UILabel::Draw()
	{
		// Adjusting text color opacity according to frame's color opacity
		if (color.alpha > layer.color.alpha)
			color.alpha = layer.color.alpha;

		Graphics::DrawTextString(layer.frame.position.x, layer.frame.position.y, layer.frame.size.width, layer.frame.size.height, Text, Properties, color);
	}
}
