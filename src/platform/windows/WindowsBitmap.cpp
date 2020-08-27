#include "WindowsBitmap.h"
#include "d2d_resources/CoreResources.h"

namespace mc
{
    Ref<Bitmap> Bitmap::Create(void* data)
    {
        return Ref<Bitmap>(new WindowsBitmap(data));
    }

    WindowsBitmap::WindowsBitmap(void* data) : Bitmap(data)
	{
	}

	WindowsBitmap::~WindowsBitmap()
	{
		((ID2D1Bitmap*)m_BmpData)->Release();
	}

	float WindowsBitmap::GetWidth() const
	{
		return ((ID2D1Bitmap*)m_BmpData)->GetSize().width;
	}

	float WindowsBitmap::GetHeight() const
	{
		return ((ID2D1Bitmap*)m_BmpData)->GetSize().height;
	}
}
