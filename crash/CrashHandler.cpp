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
#include "TypeDefinitions.h"
#include "CrashHandler.h"
#include <windows.h>
#include <iostream>
using namespace CinnamonToast;

// Internal functions
namespace {
    void NoCrashFunction() {};
    LONG WINAPI SignalHandler(EXCEPTION_POINTERS* ExceptionInfo) {
        if (ExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION) {
            std::cout << "[FATAL] [SignalHandler]: Program crashed! Segmentation fault!" << std::endl;
            std::cout << "[FATAL] [SignalHandler]: Showing message! " << std::endl;
            MessageBox(NULL, "The program crashed due to a segmentation fault. Reinstalling the app may fix the issue.", "Program crashed!", MB_ICONERROR);
            exit(0x000003f7);
        }
        return EXCEPTION_CONTINUE_SEARCH;  // Continue searching for another handler (if any)
    };
}

namespace CinnamonToast {
    CrashHandler* CrashManager::handler = nullptr;
    void CrashManager::SetActiveCrashHandler(CrashHandler* handler_){
            if (handler != nullptr) {
                handler->Deactivate();
            }
            handler = handler_;  
            handler_->Activate();
            
    }; 
    
    CrashHandler* CrashManager::GetActiveCrashHandler() {
        return handler;
    }; 
    void CrashHandler::InvokeCrash(string crashMessage) {
        if (throwCrash) {
            std::cout << "[FATAL] [InvokeUnhandledExceptionCrash]: Program crashed! " << std::endl;
            std::cout << "[FATAL] [InvokeUnhandledExceptionCrash]: Showing message! " << std::endl;
            MessageBox(NULL, "The program crashed due to an unspecified reason. Restarting the app may fix this issue.", "Program crashed!", MB_ICONERROR);
            exit(0x000003f8);
        }
    }
    bool Utilities::CheckBit(unsigned int num, int n) {
        unsigned int mask = 1 << n;  // Create a mask with 1 at the nth position
        return num & mask;  // If the nth bit is 1, the result will be non-zero
    }
    
    CrashHandler::CrashHandler(CrashConfig cf) : customCrash(NoCrashFunction), throwHandle(false), throwCrash(false), config(cf) {
    }
    void CrashHandler::InvokeUnhandledExceptionCrash(exception& ex) {
        if (throwHandle) {
            if (customCrash != NoCrashFunction) {
                customCrash();
            }
            std::cout << "[FATAL] [InvokeUnhandledExceptionCrash]: Program crashed! " << std::endl;
            std::cout << "[FATAL] [InvokeUnhandledExceptionCrash]: Printing error and showing message! " << std::endl;
            std::cout << "[FATAL] [<exception>]: " << ex.what() << std::endl;
            MessageBox(NULL, "The program crashed due to an unhandled exception. Reinstalling the app may fix this issue.", "Program crashed!", MB_ICONERROR);
            exit(0x000003f6);
        }
    };
    void CrashHandler::Activate() {
        active = true;
        if (Utilities::CheckBit(config.CrashType, 0)) {
            SetUnhandledExceptionFilter(SignalHandler);
        }
        if (Utilities::CheckBit(config.CrashType, 4)) {
            throwHandle = true;
        }
        if (Utilities::CheckBit(config.CrashType, 8)) {
            throwCrash = true;
        }
        
    }
    void CrashHandler::Deactivate() {
        active = false;
        if (Utilities::CheckBit(config.CrashType, 0)) {
            SetUnhandledExceptionFilter(NULL);
        } 
        if (Utilities::CheckBit(config.CrashType, 4)) {
            throwHandle = false;
        }
        if (Utilities::CheckBit(config.CrashType, 8)) {
            throwCrash = false;
        }

    }
    void CrashHandler::SetUnhandledExceptionCrashFunction(CrashFunction function) {
        customCrash = function;
    }
}

