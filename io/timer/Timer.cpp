#include "Timer.h"
#include "util/Binary.h"


namespace GBE
{

    Timer::Timer(const std::shared_ptr<InterruptManager> &interruptManager)
    {
        SetReadWriteFlags(true);
        m_InterruptManager = interruptManager;
    }

    Timer::~Timer()
    {
    }

    void Timer::Tick()
    {
        uint8_t div = _GetRegister(TimerRegister::DIV);
        div++;
        _SetRegister(TimerRegister::DIV, div);
        
        if (_IsTimerControlEnabled())
        {
            m_Clock++;
            if (m_Clock == _GetClockCycles())
            {
                m_Clock = 0;
                uint16_t tima = static_cast<uint16_t>(_GetRegister(TimerRegister::TIMA));
                tima++;
                if (tima == 0xFF)
                {
                    uint16_t tma = static_cast<uint16_t>(_GetRegister(TimerRegister::TMA));
                    tima = tma;
                    m_InterruptManager->QueueInterrupt(InterruptFlag::TIMER);
                    _SetRegister(TimerRegister::TIMA, static_cast<uint8_t>(tima & 0xFF));
                }
                
            }
        }
    }

    void Timer::ResetDIV()
    {
        _SetRegister(TimerRegister::DIV, 0);
    }

    void Timer::_SetImp(uint16_t address, uint8_t value)
    {
        TimerRegister reg = static_cast<TimerRegister>(address);
        if (reg == TimerRegister::DIV)
        {
            ResetDIV();
            return;
        }

        _SetRegister(reg, value);
    }

    uint8_t Timer::_GetImp(uint16_t address) const
    {
        TimerRegister reg = static_cast<TimerRegister>(address);
        return _GetRegister(reg);
    }

    uint16_t Timer::_GetClockCycles() const
    {
        uint8_t mask = 0b0011;
        uint8_t clockSelect = _GetRegister(TimerRegister::TAC) & mask;

        switch (clockSelect)
        {
        case 0:
            return 256;
        case 1:
            return 4;
        case 2:
            return 16;
        case 3:
            return 64;
        default:
            return 0;
        }
    }

    bool Timer::_IsTimerControlEnabled() const
    {
        uint8_t bit = 3;
        return Binary::TestBit(_GetRegister(TimerRegister::TAC), bit);
    }

} // namespace GBE
