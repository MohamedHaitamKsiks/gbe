#include "GBETestSuite.h"

#include "util/Assert.h"
#include "gameboy/Gameboy.h"


#define GBE_ADD_TEST_ROM(testName, successAddress) \
TEST_CASE(testName) \
{ \
    std::string romPath = "./test_roms/cpu_instrs/individual/" testName; \
    int result = GBETest::RunRomTest(romPath, successAddress, 10000); \
    CHECK_EQ(result, 0); \
}

namespace GBETest
{
    static int RunRomTest(const std::string& romPath, uint16_t successAddress, uint16_t timeoutCycles)
    {
        // load cartridge
        GBE_ASSERT(!romPath.empty());

        auto cartridge = std::make_shared<GBE::Cartridge>();
        cartridge->Load(romPath);

        // run the gameboy with the cartridge
        GBE::Gameboy gameboy{};
        gameboy.Start(cartridge);

        // 
        auto& cpu = gameboy.GetCpu();
        uint16_t cycles = 0;
        while (gameboy.IsRunning() && cycles < timeoutCycles)
        {
            uint16_t pc = cpu.GetRegisters().GetReg16(GBE::Reg16::PC);
            if (pc == successAddress)
            {
                gameboy.Stop();
                return 0; 
            }
            cycles ++; 
            gameboy.Tick();
        }

        return -1; // timeout
    }
} // namespace GBETest



GBE_TEST_SUITE(CpuRom)
{
    GBE_ADD_TEST_ROM("01-special.gb", 0xC7D2);
    // GBE_ADD_TEST_ROM("02-interrupts.gb", 0x?);
    GBE_ADD_TEST_ROM("03-op sp,hl.gb", 0xCB44);
    GBE_ADD_TEST_ROM("04-op r,imm.gb", 0xCB35);
    GBE_ADD_TEST_ROM("05-op rp.gb", 0xCB31);
    GBE_ADD_TEST_ROM("06-ld r,r.gb", 0xCC5F);
    GBE_ADD_TEST_ROM("07-jr,jp,call,ret,rst.gb", 0xCBB0);
    GBE_ADD_TEST_ROM("08-misc instrs.gb", 0xCB91);
    GBE_ADD_TEST_ROM("09-op r,r.gb", 0xCE67);
    // GBE_ADD_TEST_ROM("10-bit ops.gb", 0x?;
    GBE_ADD_TEST_ROM("11-op a,(hl).gb", 0xCC62);
}