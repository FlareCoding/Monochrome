#pragma once

namespace mc
{
	class Bitmap
	{
	public:
		Bitmap() = default;
		Bitmap(void* data);
		~Bitmap();

		const void* GetBmpData() const { return m_BmpData; }
		float GetWidth() const;
		float GetHeight() const;

	private:
		void* m_BmpData = nullptr;
	};
}
