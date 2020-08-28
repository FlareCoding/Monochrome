#pragma once
#include <graphics/Bitmap.h>

namespace mc
{
	class LinuxBitmap : public Bitmap
	{
	public:
		LinuxBitmap(void* data);
		~LinuxBitmap();

		virtual float GetWidth() const override;
		virtual float GetHeight() const override;
	};
}
