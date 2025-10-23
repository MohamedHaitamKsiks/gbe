#include "GBETestSuite.h"

#include "io/interrupts/InterruptManager.h"
#include "io/graphics/Ppu.h"

GBE_TEST_SUITE(PpuTest) 
{

    TEST_CASE("Test tile image")
    {
        auto interruptManager = std::make_shared<GBE::InterruptManager>();
        GBE::Ppu ppu(interruptManager);
        ppu.Init();
        ppu.Tick(4000);
        // ppu.Tick(13);

    }
}