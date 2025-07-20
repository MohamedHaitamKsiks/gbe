#pragma once

#include <vector>
#include <functional>

#include "PpuTask.h"

namespace GBE
{
    class PpuTaskManager
    {
    public:
        using TaskFunction = std::function<PpuTask()>;

        ~PpuTaskManager();

        // start new task
        void Start(TaskFunction taskFunc);

        // process ONE dot in 
        void ProcessDot();

        // finish running task
        void Finish();

    private:
        std::vector<PpuTask> m_Tasks{};

        PpuTask _StartTask(TaskFunction taskFunc);

        void _PopDoneTasks();
    };
} // namespace GBE
