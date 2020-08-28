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

		void SetSize(uint32_t width, uint32_t height);

	private:
		uint32_t m_Width = 0;
		uint32_t m_Height = 0;
	};
}
