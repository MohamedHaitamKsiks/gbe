#include "Joypad.h"


namespace GBE
{

    enum class JoypadFlag
    {
        A_RIGHT = 0,
        B_LEFT = 1,
        SELECT_UP = 2,
        START_DOWN = 3,
        SELECT_DPAD = 4,
        SELECT_BUTTONS = 5
    };

    Joypad::Joypad(const std::shared_ptr<InterruptManager>& interruptManager)
    {
        SetReadWriteFlags(true);
        m_InterruptManager = interruptManager;
    }

    Joypad::~Joypad()
    {
    }

    void Joypad::FireJoypadEvent(JoypadButton btn)
    {

        switch (btn)
        {
        case JoypadButton::A:
            
            break;
        
        default:
            break;
        }

        m_InterruptManager->QueueInterrupt(InterruptFlag::JOYPAD);
    }

    void Joypad::_SetImp(uint16_t address, uint8_t value)
    {
        m_JoypadFlags = value;
    }

    uint8_t Joypad::_GetImp(uint16_t address) const
    {
        return m_JoypadFlags;
    }

} // namespace GBE
