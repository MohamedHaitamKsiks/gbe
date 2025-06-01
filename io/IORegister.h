#pragma once


namespace GBE
{
    // register adresses for I/O Registers
    enum class IORegister
    {
        // Joypad
        JOYP = 0xFF00,

        // Serial
        SB = 0xFF01, // Serial transfer data
        SC = 0xFF02, // Serial transfer control

        // Timers
        DIV = 0xFF04,  // Divider register
        TIMA = 0xFF05, // Timer counter
        TMA = 0xFF06,  // Timer modulo
        TAC = 0xFF07,  // Timer control

        // Interrupts
        IF = 0xFF0F, // Interrupt flag
        IE = 0xFFFF,  // Interrupt enable

        // Sound Channel 1
        NR10 = 0xFF10, // Sweep
        NR11 = 0xFF11, // Length timer & duty cycle
        NR12 = 0xFF12, // Volume & envelope
        NR13 = 0xFF13, // Period low
        NR14 = 0xFF14, // Period high & control

        // Sound Channel 2
        NR21 = 0xFF16, // Length timer & duty cycle
        NR22 = 0xFF17, // Volume & envelope
        NR23 = 0xFF18, // Period low
        NR24 = 0xFF19, // Period high & control

        // Sound Channel 3
        NR30 = 0xFF1A, // DAC enable
        NR31 = 0xFF1B, // Length timer
        NR32 = 0xFF1C, // Output level
        NR33 = 0xFF1D, // Period low
        NR34 = 0xFF1E, // Period high & control

        // Sound Channel 4
        NR41 = 0xFF20, // Length timer
        NR42 = 0xFF21, // Volume & envelope
        NR43 = 0xFF22, // Frequency & randomness
        NR44 = 0xFF23, // Control

        // Sound control
        NR50 = 0xFF24, // Master volume & VIN panning
        NR51 = 0xFF25, // Sound panning
        NR52 = 0xFF26, // Sound on/off

        // Wave RAM
        WAVE_RAM_FROM = 0xFF30,
        WAVE_RAM_TO = 0xFF3F,

        // LCD
        LCDC = 0xFF40, // Control
        STAT = 0xFF41, // Status
        SCY = 0xFF42,  // Viewport Y
        SCX = 0xFF43,  // Viewport X
        LY = 0xFF44,   // Y coordinate
        LYC = 0xFF45,  // LY compare
        DMA = 0xFF46,  // OAM DMA

        // Palettes (DMG)
        BGP = 0xFF47,  // BG palette
        OBP0 = 0xFF48, // OBJ palette 0
        OBP1 = 0xFF49, // OBJ palette 1

        // Window
        WY = 0xFF4A, // Y position
        WX = 0xFF4B, // X position + 7
    };
} // namespace GBE
