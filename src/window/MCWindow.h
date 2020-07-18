#ifndef MCWINDOW_H
#define MCWINDOW_H
#include <memory>
#include <cinttypes>

namespace mc
{

class MCWindow
{
public:
	static std::shared_ptr<MCWindow> Create(uint32_t width, uint32_t height, const char* title);

	virtual uint32_t GetWidth() = 0;
	virtual uint32_t GetHeight() = 0;
};

}
#endif
