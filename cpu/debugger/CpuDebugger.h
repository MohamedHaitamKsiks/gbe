#pragma once

#include <cstdint>
#include <flat_set>

namespace GBE
{
    class CpuDebugger
    {
    public:
        CpuDebugger();
        ~CpuDebugger() = default;
        
        CpuDebugger(const CpuDebugger&) = delete;               
        CpuDebugger& operator=(const CpuDebugger&) = delete;

        void Init();

        void AddBreakPoint(uint16_t address);
        void RemoveBreakPoint(uint16_t address);

        inline const std::flat_set<uint16_t>& GetBreakPoints() const
        {
            return m_BreakPoints;
        }

        // returns if paused or not
        void Tick(uint16_t address);

        void Start();
        void Stop();
        void Step();
        void Continue();

        inline bool IsEnabled() const
        {
            return m_Enabled;
        }

        bool IsBreaked() const ;
    private:
        std::flat_set<uint16_t> m_BreakPoints{};
        bool m_Enabled = false;
        bool m_Breaked = false;
        bool m_Continue = false;
        bool m_Step = false;
    };
} // namespace GBE
