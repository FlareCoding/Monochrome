#include "StackTracer.h"
#include "Core.h"

#ifdef MC_PLATFORM_WINDOWS
#include <Windows.h>
#include <DbgHelp.h>
#pragma comment(lib, "Dbghelp.lib")

void _printDebugStackTrace() {
    HANDLE process = GetCurrentProcess();
    SymInitialize(process, NULL, TRUE);
    SymSetOptions(SYMOPT_LOAD_LINES);

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
        // Get the filepath and line information
        DWORD  dwDisplacement;
        IMAGEHLP_LINE64 line;
        SymGetLineFromAddr64(process, (DWORD64)stack[i], &dwDisplacement, &line);

        // Extract only the filename
        std::string filepath = line.FileName;
        std::string filename = filepath.substr(filepath.find_last_of("/\\") + 1);

        // Get the function name and address
        SymFromAddr(process, (DWORD64)(stack[i]), 0, symbol);

        std::cout << '\t' <<
            frames - i - 1 << ':' << symbol->Name <<
            reinterpret_cast<void*>(symbol->Address) <<
            " [" << filename << ':' << line.LineNumber << "]\n";
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
