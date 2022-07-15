#include "uuid.h"
#include <random>

namespace mc::utils {
    uuid_t generateUUID() {
        static std::random_device rd;
        static std::mt19937 engine(rd());

        std::uniform_int_distribution<std::uint64_t> distribution(
            std::numeric_limits<std::uint64_t>::min(),
            std::numeric_limits<std::uint64_t>::max()
        );

        return distribution(engine);
    }
} // namespace mc::utils
