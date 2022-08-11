#pragma once
#include <core/Core.h>
#include <string>
#include <locale>
#include <codecvt>

namespace mc::utils {
    extern std::wstring convertToWideString(const std::string& str);
    extern std::string convertToNarrowString(const std::wstring& wstr);
} // namespace mc::utils
