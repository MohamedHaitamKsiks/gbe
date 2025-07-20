#include "GBETestSuite.h"

#include "io/graphics/PpuTaskManager.h"

GBE_TEST_SUITE(PpuTaskTest)
{
    enum class TaskState
    {
        NONE = 0,
        START,
        DOT_PROCESS,
        WAIT_PROCESS,
        DONE,
    };


    TEST_CASE("Start")
    {
        // arrange
        auto state = TaskState::NONE;
        GBE::PpuTaskManager manager{};

        auto task = [&]() -> GBE::PpuTask {
            state = TaskState::START;
            co_return;
        };

        // act
        manager.Start(task);

        // assert
        CHECK_EQ(state, TaskState::START);
    }

    TEST_CASE("ProcessDot")
    {
        // arrange
        auto state = TaskState::NONE;
        GBE::PpuTaskManager manager{};

        auto task = [&]() -> GBE::PpuTask
        {
            co_yield nullptr;
            state = TaskState::DOT_PROCESS;
            co_return;
        };

        // act
        manager.Start(task);

        // assert
        CHECK_NE(state, TaskState::DOT_PROCESS);

        manager.ProcessDot();
        CHECK_EQ(state, TaskState::DOT_PROCESS);
    }

    TEST_CASE("Wait")
    {
        // arrange
        auto state = TaskState::NONE;
        GBE::PpuTaskManager manager{};

        auto task = [&]() -> GBE::PpuTask
        {
            co_yield GBE::PpuTask::Wait(4);
            state = TaskState::WAIT_PROCESS;
            co_return;
        };

        // act
        manager.Start(task);
        manager.ProcessDot(); // still should be at start

        // assert
        CHECK_NE(state, TaskState::WAIT_PROCESS);

        // need two more dots to end the wait
        for (uint32_t i = 0; i < 3; i++)
            manager.ProcessDot();
        CHECK_EQ(state, TaskState::WAIT_PROCESS);
    }
}