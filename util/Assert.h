#pragma once
#include <exception>
#include <stacktrace>


#ifdef GBE_DISABLE_ASSERTS
    #define GBE_ASSERT(condition)
#else
    #define GBE_ASSERT(condition) GBE::AssertImpl((condition), #condition, __FILE__, __LINE__)
#endif

namespace GBE
{
    void AssertImpl(bool condition, const char *conditionStr, const char *file, int line);
} // namespace GBE
