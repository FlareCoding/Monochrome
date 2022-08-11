#include "InternalFlags.h"

namespace mc {
    void setInternalFlag(uint64_t& flags, uint64_t flagToSet, bool enabled) {
        if (enabled) {
            flags |= (1ULL << static_cast<uint64_t>(flagToSet));
        } else {
            flags &= ~(1ULL << static_cast<uint64_t>(flagToSet));
        }
    }

    bool getInternalFlag(uint64_t flags, uint64_t flagToGet) {
        return (flags >> static_cast<uint64_t>(flagToGet)) & 1ULL;
    }
} // namespace mc
