#pragma once
#include <core/Core.h>

namespace mc
{
	class Bitmap
	{
	public:
		static Ref<Bitmap> Create(void* data);

		Bitmap(void* data) : m_BmpData(data) {}
		virtual ~Bitmap() = default;

		const void* GetBmpData() const { return m_BmpData; }
		virtual float GetWidth() const = 0;
		virtual float GetHeight() const = 0;

	protected:
		void* m_BmpData = nullptr;
	};
}
