#include "TextUtils.h"

namespace mc::utils
{
    std::wstring convertToWideString(const std::string& str) {
        return std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(str);
    }

    std::string convertToNarrowString(const std::wstring& wstr) {        
        return std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(wstr);
    }
}
