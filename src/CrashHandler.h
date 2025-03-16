/**
* Copyright (c) 2025 thebest12lines
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software
* and associated documentation files (the "Software"), to deal in the Software without restriction,
* including without limitation the rights to use, copy, modify, merge, publish, distribute,
* sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
* BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
* DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once
#include <windows.h>
#include "TypeDefinitions.h"
#include <cstdint>
#include <string>

#define crash_unhandled_exception 0x10
#define crash_invoke 0x100
#define crash_segfault 0x1
typedef void (*CrashFunction)();

namespace CinnamonToast {
    struct CrashConfig {
        uint32_t MessageType;
        uint32_t CrashType;
    };
    class CrashHandler {
        private:
            CrashFunction customCrash;
            bool active;
            bool throwHandle;
            bool throwCrash;
            CrashConfig config;
            __declspec(dllimport) void Activate();
            __declspec(dllimport) void Deactivate();
            
        public:
            __declspec(dllimport) CrashHandler(CrashConfig cf);
            __declspec(dllimport) void SetCustomCrashHandler(CrashFunction func);
            __declspec(dllimport) void InvokeCrash(string crashMessage);

            // Note: explicitly need to define try-catch block and to call this method
            __declspec(dllimport) void InvokeUnhandledExceptionCrash(exception& ex);
            __declspec(dllimport) void SetUnhandledExceptionCrashFunction(CrashFunction function);

            friend class CrashManager;
        
    };
    class CrashManager {
        private:
            static CrashHandler* handler;
        public:
            __declspec(dllimport) static void SetActiveCrashHandler(CrashHandler* handler_);
            __declspec(dllimport) static CrashHandler* GetActiveCrashHandler();
    }; 
    namespace Utilities {
        bool CheckBit(unsigned int num, int n);
    }
}