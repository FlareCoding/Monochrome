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
		return m_Width;
	}

	float LinuxBitmap::GetHeight() const
	{
		return m_Height;
	}

	void LinuxBitmap::SetSize(uint32_t width, uint32_t height)
	{
		m_Width = width;
		m_Height = height;
	}
}
