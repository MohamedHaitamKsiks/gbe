#include "PpuTaskManager.h"

namespace GBE
{
    PpuTaskManager::~PpuTaskManager()
    {
    }

    void PpuTaskManager::Start(TaskFunction taskFunc)
    {
        m_Tasks.reserve(100);
        
        auto task = _StartTask(taskFunc);
        m_Tasks.push_back(task);
        
        ProcessDot();
    }

    void PpuTaskManager::ProcessDot()
    {
        _PopDoneTasks();

        if (m_Tasks.size() == 0)
            return;

        auto task = m_Tasks.back();
        while (task.promise().CurrentTaskSet)
        {
            m_Tasks.push_back(task.promise().CurrentTask);
            task = task.promise().CurrentTask;
        }

        task.promise().Dots++;
    }

    void PpuTaskManager::Finish()
    {
        while (m_Tasks.size() > 0)
        {
            auto task = m_Tasks.back();
            auto& promise = task.promise();
            while (!promise.Done)
                task();

            m_Tasks.pop_back();
        }
    }

    PpuTask PpuTaskManager::_StartTask(TaskFunction taskFunc)
    {
        co_yield nullptr;
        co_yield taskFunc();
        co_return;
    }

    void PpuTaskManager::_PopDoneTasks()
    {
        uint32_t lastDots = 0;
        while (m_Tasks.size() > 0)
        {
            auto task = m_Tasks.back();
            auto &promise = task.promise();
            if (promise.Done)
            {
                lastDots = promise.Dots;
                m_Tasks.pop_back();

                // update dots
                if (m_Tasks.size() > 0)
                    m_Tasks.back().promise().Dots += lastDots;

                continue;
            }

            task();
            if (!promise.Done)
                break;
        }
    }

} // namespace GBE
