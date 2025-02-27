#include "log.hpp"

std::mutex debug::log_mutex;
std::atomic < decltype(std::cout)* > debug::output = &std::cerr;

#ifdef __DEBUG__

#include <crtdbg.h>
#include <windows.h>
#include <dbghelp.h>
#include <iostream>
#include <cstdlib>
#include <sstream>

class enable_mem_leak_check
{
public:
	enable_mem_leak_check()
	{
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
        debug::output = &std::cout;
	}
} enable_mem_leak_check_;

// Function to print the call stack
std::string debug::GetStackTrace() 
{
    std::stringstream ret;
    CONTEXT context;
    ZeroMemory(&context, sizeof(context));
    RtlCaptureContext(&context);  // Capture the current context

    // Initialize the symbol handler
    if (!SymInitialize(GetCurrentProcess(), NULL, TRUE)) {
        ret << "Error initializing symbol handler: " << GetLastError() << std::endl;
    }

    // Set up the stack frame
    STACKFRAME64 stackFrame;
    ZeroMemory(&stackFrame, sizeof(stackFrame));
    stackFrame.AddrPC.Offset = context.Rip;
    stackFrame.AddrPC.Mode = AddrModeFlat;
    stackFrame.AddrFrame.Offset = context.Rbp;
    stackFrame.AddrFrame.Mode = AddrModeFlat;
    stackFrame.AddrStack.Offset = context.Rsp;
    stackFrame.AddrStack.Mode = AddrModeFlat;

    // Walk the stack
    int frameCount = 0;
    while (StackWalk64(
        IMAGE_FILE_MACHINE_AMD64,
        GetCurrentProcess(),
        GetCurrentThread(),
        &stackFrame,
        &context,
        nullptr,
        SymFunctionTableAccess64,
        SymGetModuleBase64,
        nullptr))
    {
        DWORD64 symbolAddress = stackFrame.AddrPC.Offset;
        char symbolBuffer[sizeof(SYMBOL_INFO) + 256];  // Allocate space for the symbol info
        SYMBOL_INFO* symbol = (SYMBOL_INFO*)symbolBuffer;
        symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
        symbol->MaxNameLen = 255;
        DWORD64 displacement = 0;

        if (frameCount != 0) 
        {
            if (SymFromAddr(GetCurrentProcess(), symbolAddress, &displacement, symbol))
            {
                ret << "Frame " << frameCount << ": " << symbol->Name
                    << " (Adr=0x" << std::hex << symbol->Address
                    << ", Disp=0x" << std::hex << displacement << ")" << std::endl;
            }
            else
            {
                ret << "Frame " << frameCount << ": Error while getting symbol: " << GetLastError() << std::endl;
            }
        }

        frameCount++;
    }

    // Clean up the symbol handler
    SymCleanup(GetCurrentProcess());

    return ret.str();
}

#endif // __DEBUG__
