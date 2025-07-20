#include "PpuTask.h"

namespace GBE
{
    PpuTask PpuTask::Wait(uint32_t dots)
    {
        for (uint32_t i = 0; i < dots; i++)
        {
            co_yield nullptr;
        }
        
        co_return;
    }

} // namespace GBE
