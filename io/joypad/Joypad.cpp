#include "Joypad.h"

#include "util/Binary.h"


namespace GBE
{
    constexpr uint8_t JOYPAD_BUTTON_TYPE_MASK = 0b00110000;

    Joypad::Joypad(const std::shared_ptr<InterruptManager>& interruptManager)
    {
        SetReadWriteFlags(true);
        m_InterruptManager = interruptManager;

        m_JoypadMatrix[JoypadButtonType::SELECT_DPAD] = 0xFF;
        m_JoypadMatrix[JoypadButtonType::SELECT_BUTTONS] = 0xFF;

        // joypad map
        // buttons
        m_JoypadInfos[JoypadButton::A] = JoypadButtonInfo{
            .Flag = JoypadButtonFlag::A_RIGHT, 
            .Type = JoypadButtonType::SELECT_BUTTONS
        };

        m_JoypadInfos[JoypadButton::B] = JoypadButtonInfo{
            .Flag = JoypadButtonFlag::B_LEFT, 
            .Type = JoypadButtonType::SELECT_BUTTONS
        };

        m_JoypadInfos[JoypadButton::SELECT] = JoypadButtonInfo{
            .Flag = JoypadButtonFlag::SELECT_UP, 
            .Type = JoypadButtonType::SELECT_BUTTONS
        };

        m_JoypadInfos[JoypadButton::START] = JoypadButtonInfo{
            .Flag = JoypadButtonFlag::START_DOWN, 
            .Type = JoypadButtonType::SELECT_BUTTONS
        };

        // arrows
        m_JoypadInfos[JoypadButton::LEFT] = JoypadButtonInfo{
            .Flag = JoypadButtonFlag::B_LEFT, 
            .Type = JoypadButtonType::SELECT_DPAD
        };

        m_JoypadInfos[JoypadButton::RIGHT] = JoypadButtonInfo{
            .Flag = JoypadButtonFlag::A_RIGHT, 
            .Type = JoypadButtonType::SELECT_DPAD
        };

        m_JoypadInfos[JoypadButton::UP] = JoypadButtonInfo{
            .Flag = JoypadButtonFlag::SELECT_UP,
            .Type = JoypadButtonType::SELECT_DPAD
        };

        m_JoypadInfos[JoypadButton::DOWN] = JoypadButtonInfo{
            .Flag = JoypadButtonFlag::START_DOWN, 
            .Type = JoypadButtonType::SELECT_DPAD
        };
    }

    Joypad::~Joypad()
    {
    }

    void Joypad::QueueJoypadEvent(JoypadEvent event)
    {
        m_Events.push_back(event);
    }

    void Joypad::Tick()
    {
        if (m_Events.empty())
            return;

        JoypadEvent &top = m_Events.back();
        const JoypadButtonInfo& info = m_JoypadInfos.at(top.Button);
        
        // get current state
        uint8_t flags = m_JoypadMatrix.at(info.Type);
        bool currentState = !Binary::TestBit(flags, static_cast<uint8_t>(info.Flag));
        
        // fire interrupt
        if (! currentState && top.Pressed)
        {
            m_InterruptManager->QueueInterrupt(InterruptFlag::JOYPAD);
        }

        // update state
        if (top.Pressed)
            flags = Binary::ResetBit(flags, static_cast<uint8_t>(info.Flag));
        else
            flags = Binary::SetBit(flags, static_cast<uint8_t>(info.Flag));

        m_JoypadMatrix[info.Type] = flags;
        m_Events.pop_back();
    }

    void Joypad::_SetImp(uint16_t address, uint8_t value)
    {
        uint8_t mask = JOYPAD_BUTTON_TYPE_MASK;
        m_JoypadFlags = (value & mask) | m_JoypadFlags;
    }

    uint8_t Joypad::_GetImp(uint16_t address) const
    {
        JoypadButtonType type = _GetType();
        uint8_t mask = JOYPAD_BUTTON_TYPE_MASK;

        return (m_JoypadFlags & mask) | m_JoypadMatrix.at(type);
    }

    Joypad::JoypadButtonType Joypad::_GetType() const
    {   
        if (!Binary::TestBit(m_JoypadFlags, static_cast<uint8_t>(JoypadButtonType::SELECT_DPAD)))
        {
            return JoypadButtonType::SELECT_DPAD;
        }

        return JoypadButtonType::SELECT_BUTTONS;
    }

} // namespace GBE
