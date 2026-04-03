#include "GBETestSuite.h"

#include "io/interrupts/InterruptManager.h"
#include "io/joypad/Joypad.h"

#include "util/Binary.h"

GBE_TEST_SUITE(JoypadTest)
{

    TEST_CASE("QueueEvent and Tick should fire interrupt")
    {
        // arrange
        auto interruptManager = std::make_shared<GBE::InterruptManager>();
        uint8_t joypadInterruptBit = static_cast<uint8_t>(GBE::InterruptFlag::JOYPAD);
        
        GBE::Joypad joypad(interruptManager);

        // act
        joypad.QueueJoypadEvent(GBE::JoypadEvent{
            .Button = GBE::JoypadButton::A,
            .Pressed = true
        });
        joypad.Tick();
        uint8_t interruptFlag = interruptManager->GetInterruptFlag();

        // assert
        CHECK(GBE::Binary::TestBit(interruptFlag, joypadInterruptBit))    ;
    }

    TEST_CASE("QueueEvent select button and check byte value")
    {
        // arrange
        auto interruptManager = std::make_shared<GBE::InterruptManager>();
        uint8_t expectedValue = 0b00011110; //  xx01 1110 - select buttons and A pressed

        GBE::Joypad joypad(interruptManager);
        joypad.Set(0, 0b00010000); // xx01 0000 - select buttons

        // act
        joypad.QueueJoypadEvent(GBE::JoypadEvent{
            .Button = GBE::JoypadButton::A,
            .Pressed = true
        });
        joypad.Tick();
        uint8_t value = joypad.Get(0);

        // assert
        CHECK_EQ(value, expectedValue);
    }

    TEST_CASE("QueueEvent d-pad event and check byte value")
    {
        // arrange
        auto interruptManager = std::make_shared<GBE::InterruptManager>();
        uint8_t expectedValue = 0b00101011; //  xx10 1011 - select d-pad and A and UP pressed

        GBE::Joypad joypad(interruptManager);
        joypad.Set(0, 0b00100000); // xx10 0000 - d-pad buttons

        // act
        joypad.QueueJoypadEvent(GBE::JoypadEvent{
            .Button = GBE::JoypadButton::UP,
            .Pressed = true
        });
        
        joypad.Tick();
        uint8_t value = joypad.Get(0);

        // assert
        CHECK_EQ(value, expectedValue);
    }
}