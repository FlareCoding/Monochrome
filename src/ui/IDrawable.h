#pragma once

namespace mc
{
	class IDrawable
	{
	public:
		/// Determines whether or not to draw the element.
		bool Visible = true;

		/// Called during the graphics update routine.
		virtual void Draw() = 0;

		virtual ~IDrawable() = default;
	};
}
