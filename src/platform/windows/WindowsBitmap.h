#pragma once
#include <graphics/Bitmap.h>

namespace mc
{
	class WindowsBitmap : public Bitmap
	{
	public:
		WindowsBitmap(void* data);
		~WindowsBitmap();

		virtual float GetWidth() const override;
		virtual float GetHeight() const override;
	};
}
