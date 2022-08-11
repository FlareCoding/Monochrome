#include "StackTracer.h"
#include "Core.h"

#ifdef MC_PLATFORM_WINDOWS
#include <Windows.h>
#include <DbgHelp.h>
#pragma comment(lib, "Dbghelp.lib")

void _printDebugStackTrace() {
    HANDLE process = GetCurrentProcess();
    SymInitialize(process, NULL, TRUE);

    void* stack[64];
    uint16_t frames = CaptureStackBackTrace(0, 64, stack, NULL);

    SYMBOL_INFO* symbol =
        reinterpret_cast<SYMBOL_INFO*>(calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char), 1));

    if (!symbol) {
        return;
    }

    symbol->MaxNameLen = 255;
    symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

    std::cout << "Stacktrace:\n";
    for (int i = 0; i < frames; i++) {
        SymFromAddr(process, (DWORD64)(stack[i]), 0, symbol);
        std::cout << '\t' <<
            frames - i - 1 << ':' << symbol->Name <<
            reinterpret_cast<void*>(symbol->Address) << "\n";
    }

    free(symbol);
}
#else
#include <execinfo.h>

void _printDebugStackTrace() {
    std::cerr << "Stacktrace:\n";

    void* callstack[128];
    int i, frames = backtrace(callstack, 128);
    char** strs = backtrace_symbols(callstack, frames);

    for (i = 0; i < frames; ++i) {
            printf("%s\n", strs[i]);
    }

    free(strs);
}
#endif
