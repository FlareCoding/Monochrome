#pragma once

namespace mc
{
	class IDrawable
	{
	public:
		bool Visible = true;

		// Called during the graphics update routine
		virtual void Draw() = 0;

		virtual ~IDrawable() = default;
	};
}
