#include "OSXBitmap.h"
#include "OSXNativeCore.h"

namespace mc
{
    Ref<Bitmap> Bitmap::Create(void* data)
    {
        return Ref<Bitmap>(new OSXBitmap(data));
    }

    OSXBitmap::OSXBitmap(void* data) : Bitmap(data)
	{
	}

	OSXBitmap::~OSXBitmap()
	{
	}

	float OSXBitmap::GetWidth() const
	{
		return reinterpret_cast<NSImage*>(m_BmpData).size.width;
	}

	float OSXBitmap::GetHeight() const
	{
		return reinterpret_cast<NSImage*>(m_BmpData).size.height;
	}
}
