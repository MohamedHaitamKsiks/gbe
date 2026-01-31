#include "Assert.h"

#include <sstream>
#include <cxxabi.h>
#include <execinfo.h>
#include <print>

namespace GBE
{
    std::string DemangleFunctionName(const std::string &functionName)
    {
        std::string demangledName = functionName;

        // extract mangled name
        size_t begin = functionName.find('(');
        size_t end = functionName.find('+', begin);
        bool canDemangle = (begin != std::string::npos && end != std::string::npos);

        if (!canDemangle)
            return demangledName;
        
            
            
        // try to demangle
        std::string mangledFunctionName = functionName.substr(begin + 1, end - begin - 1);
        
        int status;
        char *demangled = abi::__cxa_demangle(mangledFunctionName.c_str(), nullptr, nullptr, &status);
        if (status == 0)
        {
            demangledName = std::string(demangled);
            free(demangled);
        }
    
        return demangledName;
    }

    std::string PrintStackTrace()
    {
        std::stringstream ss;
        constexpr int maxFrames = 100;
        constexpr int skipFrames = 2; // skip PrintStackTrace and AssertImpl frames

        void *array[maxFrames];
        size_t size = backtrace(array, maxFrames);
        char** pStrings = backtrace_symbols(array, size);

        for (size_t i = skipFrames; i < size; i++)
        {
            // print line
            if (i == skipFrames)
                ss << "--> ";
            else
                ss << "    ";

            ss << DemangleFunctionName(pStrings[i]) << "\n";
        }
        free(pStrings);
        return ss.str();
    }

    void AssertImpl(bool condition, const char *conditionStr, const char *file, int line)
    {
        if (condition)
            return;

        std::string stackTrace = PrintStackTrace();
        throw std::runtime_error(
            "\nAssertion failed: (" + std::string(conditionStr) + ")\nFile: " + std::string(file) +
            ":" + std::to_string(line) + "\nStack trace:\n" + stackTrace);
    
    }

} // namespace GBE
