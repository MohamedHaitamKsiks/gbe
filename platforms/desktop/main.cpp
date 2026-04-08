#include "frontend/Application.h"

#include <chrono>
#include <thread>

int main()
{
    constexpr float SEC_TO_MICRO = 1000000.0f;
    GBE::Application app{};

    float delta = 0.0f;
    while (app.IsRunning())
    {
        const auto pastTime = std::chrono::high_resolution_clock::now();
        
        app.Update(delta);
        
        const auto currentTime = std::chrono::high_resolution_clock::now();
        delta = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - pastTime).count() / SEC_TO_MICRO;
    }

    return 0;
}