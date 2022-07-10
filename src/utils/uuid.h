#pragma once
#include <core/Core.h>

namespace mc
{
    using uuid_t = std::uint64_t;

    namespace utils
    {
        uuid_t generateUUID();
    }
}
