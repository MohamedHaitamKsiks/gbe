#pragma once

#include <iostream>
#include <coroutine>


namespace GBE
{
    struct PpuPromise;

    struct PpuTask : std::coroutine_handle<PpuPromise>
    {
        using promise_type = PpuPromise;

        static PpuTask Wait(uint32_t dots);
    };

    struct PpuPromise
    {
        PpuTask CurrentTask;
        bool CurrentTaskSet = false;
        bool Done = false;
        uint32_t Dots = 0;

        PpuTask get_return_object() { return {PpuTask::from_promise(*this)}; }
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_never final_suspend() noexcept {

            return {}; 
        }
        std::suspend_always yield_value(PpuTask value) 
        { 
            CurrentTask = value;
            CurrentTaskSet = true;
            return {}; 
        }

        std::suspend_always yield_value(std::nullptr_t)
        {
            CurrentTaskSet = false;
            return {};
        }

        void return_void() 
        {
            Done = true;
        }
        void unhandled_exception() {}
    };
} // namespace GBE
