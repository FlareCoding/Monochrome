#ifndef WIN_32_WINDOW_H
#define WIN_32_WINDOW_H
#include <window/MCWindow.h>
#include <Windows.h>

namespace mc
{

class Win32Window : public MCWindow
{
	uint32_t m_Width = 0, m_Height = 0;

public:
	Win32Window(uint32_t width, uint32_t height, const char* title);

	// Inherited via MCWindow
	virtual uint32_t GetWidth() override;
	virtual uint32_t GetHeight() override;
};

}

#endif