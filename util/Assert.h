#pragma once

#ifdef GBE_DISABLE_ASSERTS
    #define GBE_ASSERT(condition)
#else
    #define GBE_ASSERT(condition) GBE::Assert((condition), #condition, __FILE__, __LINE__)
#endif

namespace GBE
{
    void AssertImpl(bool condition, const char *conditionStr, const char *file, int line);
    
    template<typename T>
    void Assert(T condition, const char *conditionStr, const char *file, int line)
    {
        if (condition)
            return;

        AssertImpl(false, conditionStr, file, line);
    }

} // namespace GBE
