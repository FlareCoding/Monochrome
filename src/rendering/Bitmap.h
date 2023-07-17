#pragma once
#include <core/Core.h>
#include <string>

namespace mc {
class Bitmap {
public:
    Bitmap(uint32_t width, uint32_t height, void* data)
        : d_width(width), d_height(height), d_data(data) {}

    ~Bitmap();

    inline uint32_t getWidth() const { return d_width; }
    inline uint32_t getHeight() const { return d_height; }
    inline void* getData() const { return d_data; }

    static Shared<Bitmap> loadFromFile(const std::string& path);
    static Shared<Bitmap> loadFromWebUrl(const std::string& url);

private:
    uint32_t    d_width = 0;
    uint32_t    d_height = 0;
    void*       d_data = nullptr;
};
} // namespace mc
