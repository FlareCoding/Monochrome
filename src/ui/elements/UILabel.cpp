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
		color.alpha = layer.color.alpha;

		if (UseWidestringText)
			Graphics::DrawTextWideString(layer.frame.position.x, layer.frame.position.y, layer.frame.size.width, layer.frame.size.height, WidestringText, Properties, color);
		else
			Graphics::DrawTextString(layer.frame.position.x, layer.frame.position.y, layer.frame.size.width, layer.frame.size.height, Text, Properties, color);
	}
}
