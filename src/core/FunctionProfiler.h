#pragma once
#include "Core.h"
#include <chrono>
#include <thread>
#include <map>

namespace mc::debug {
class FunctionProfiler {
public:
	FunctionProfiler(const char* fnName);
	~FunctionProfiler();

	void endProfiling();

private:
	const char* d_fnName;
	std::chrono::time_point<std::chrono::high_resolution_clock> d_startTimestamp;
	bool d_running;
};

struct ProfilerRecord {
	ProfilerRecord() = default;
	ProfilerRecord(
		const std::string& fnName,
		uint64_t duration,
		uint64_t threadId
	) : functionName(fnName), durationInMicroseconds(duration), threadId(threadId) {}

	std::string functionName;
	uint64_t durationInMicroseconds;
	uint64_t threadId;
};

class GlobalFunctionProfilerRegistry {
public:
	static GlobalFunctionProfilerRegistry& get();

	void recordProfilerSession(ProfilerRecord& record);
	ProfilerRecord& getProfilerSessionRecord(const std::string& fnName);

private:
	std::map<std::string, ProfilerRecord> d_profilerRecords;
};

#define __PROFILER_NAME_CAT(a, b) a ## b
#define PROFILER_NAME_CAT(a, b) __PROFILER_NAME_CAT(a, b)
#define PROFILER_NAME() PROFILER_NAME_CAT(profiler_, __LINE__)

#define PROFILE_FUNCTION(fnName) mc::debug::FunctionProfiler PROFILER_NAME()(fnName)
} // namespace mc::debug
