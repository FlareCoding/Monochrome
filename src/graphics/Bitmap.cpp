#include "Bitmap.h"

#if defined(_WIN32)
#include <graphics/d2d_resources/CoreResources.h>

namespace mc
{
	Bitmap::Bitmap(void* data) : m_BmpData(data)
	{
	}

	Bitmap::~Bitmap()
	{
		((ID2D1Bitmap*)m_BmpData)->Release();
	}

	float Bitmap::GetWidth() const
	{
		return ((ID2D1Bitmap*)m_BmpData)->GetSize().width;
	}

	float Bitmap::GetHeight() const
	{
		return ((ID2D1Bitmap*)m_BmpData)->GetSize().height;
	}
}
#elif defined(__APPLE__)
namespace mc
{
	Bitmap::Bitmap(void* data) : m_BmpData(data)
	{
	}

	Bitmap::~Bitmap()
	{
		
	}

	float Bitmap::GetWidth() const
	{
		return 0;
	}

	float Bitmap::GetHeight() const
	{
		return 0;
	}
}
#elif defined(__linux__)
namespace mc
{
	Bitmap::Bitmap(void* data) : m_BmpData(data)
	{
	}

	Bitmap::~Bitmap()
	{
		
	}

	float Bitmap::GetWidth() const
	{
		return 0;
	}

	float Bitmap::GetHeight() const
	{
		return 0;
	}
}
#endif
