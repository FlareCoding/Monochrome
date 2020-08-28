#include "LinuxBitmap.h"

namespace mc
{
    Ref<Bitmap> Bitmap::Create(void* data)
    {
        return Ref<Bitmap>(new LinuxBitmap(data));
    }

    LinuxBitmap::LinuxBitmap(void* data) : Bitmap(data)
	{
	}

	LinuxBitmap::~LinuxBitmap()
	{
	}

	float LinuxBitmap::GetWidth() const
	{
		return 0;
	}

	float LinuxBitmap::GetHeight() const
	{
		return 0;
	}
}
