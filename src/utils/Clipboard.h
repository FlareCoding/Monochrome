#pragma once
#include <core/Core.h>
#include <string>

namespace mc::utils
{
    class Clipboard
    {
    public:
        static void saveToClipboard(const std::string& data);
        static std::string getClipboardText();
    };
}
