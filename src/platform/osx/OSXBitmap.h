#pragma once
#include <graphics/Bitmap.h>

namespace mc
{
	class OSXBitmap : public Bitmap
	{
	public:
		OSXBitmap(void* data);
		~OSXBitmap();

		virtual float GetWidth() const override;
		virtual float GetHeight() const override;
	};
}
