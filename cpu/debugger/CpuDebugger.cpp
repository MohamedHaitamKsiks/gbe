#include "CpuDebugger.h"

#include "io/interrupts/InterruptFlag.h"
#include "io/interrupts/InterruptManager.h"

namespace GBE
{
    CpuDebugger::CpuDebugger()
    {
        AddBreakPoint(0x100);

        // add interrupt flags
        std::vector<InterruptFlag> flags = {
            InterruptFlag::V_BLANK,
            InterruptFlag::LCD,
            InterruptFlag::TIMER,
            InterruptFlag::SERIAL,
            InterruptFlag::JOYPAD
        };

        for (auto flag: flags)
        {
            uint16_t handler = InterruptManager::GetHandler(flag);
            AddBreakPoint(handler);
        }
    }

    void CpuDebugger::Init()
    {
        m_Breaked = false;
        m_Step = false;
        m_Continue = false;
    }

    void CpuDebugger::AddBreakPoint(uint16_t address)
    {
        m_BreakPoints.insert(address);
    }

    void CpuDebugger::RemoveBreakPoint(uint16_t address)
    {
        m_BreakPoints.erase(address);
    }

    void CpuDebugger::Tick(uint16_t address)
    {
        if (!m_Enabled || m_Breaked)
            return;

        if (m_Continue)
        {
            m_Continue = false;

            return;
        }

        if ((m_Step || m_BreakPoints.contains(address)))
        {
            m_Step = false;
            m_Breaked = true;
        }
    }

    void CpuDebugger::Start()
    {
        m_Enabled = true;
    }

    void CpuDebugger::Stop()
    {
        m_Enabled = false;
    }

    void CpuDebugger::Step()
    {
        Continue();
        m_Step = true;
    }

    void CpuDebugger::Continue()
    {
        m_Continue = true;
        m_Breaked = false;
    }

    bool CpuDebugger::IsBreaked() const
    {
        return m_Breaked;
    }

} // namespace GBE
