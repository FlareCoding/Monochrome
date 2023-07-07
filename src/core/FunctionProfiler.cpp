#include "FunctionProfiler.h"

namespace mc::debug {
    FunctionProfiler::FunctionProfiler(const char* fnName) {
        d_running = true;
        d_fnName = fnName;

        d_startTimestamp = std::chrono::high_resolution_clock::now();
    }

    FunctionProfiler::~FunctionProfiler() {
        if (d_running) {
            endProfiling();
        }
    }

    void FunctionProfiler::endProfiling() {
        auto endTimepoint = std::chrono::high_resolution_clock::now();

        int64_t start = std::chrono::time_point_cast<std::chrono::microseconds>(
            d_startTimestamp
        ).time_since_epoch().count();

        int64_t end = std::chrono::time_point_cast<std::chrono::microseconds>(
            endTimepoint
        ).time_since_epoch().count();

        d_running = false;

        uint64_t threadId = std::hash<std::thread::id>{}(std::this_thread::get_id());
        uint64_t duration = static_cast<uint64_t>(end - start);

        ProfilerRecord record;
        record.functionName = d_fnName;
        record.durationInMicroseconds = duration;
        record.threadId = threadId;

        GlobalFunctionProfilerRegistry::get().recordProfilerSession(record);
    }

    GlobalFunctionProfilerRegistry& GlobalFunctionProfilerRegistry::get() {
        static auto s_globalRegistry = MakeRef<GlobalFunctionProfilerRegistry>();
        return *s_globalRegistry;
    }

    void GlobalFunctionProfilerRegistry::recordProfilerSession(ProfilerRecord& record) {
        d_profilerRecords[record.functionName] = record;
    }

    ProfilerRecord& GlobalFunctionProfilerRegistry::getProfilerSessionRecord(
        const std::string& fnName
    ) {
        static ProfilerRecord s_limboRecord = ProfilerRecord("Unknown", 0, 0);

        if (d_profilerRecords.find(fnName) != d_profilerRecords.end()) {
            return d_profilerRecords[fnName];
        }

        return s_limboRecord;
    }
} // namespace mc::debug
