#include "common.h"
#include <hwlib/counter.h>

using namespace ch::htl;

TEST_CASE("counter", "[counter]") {
  SECTION("counter", "[counter]") {
    TESTX([]()->bool {
      ch_device<counter<4>> device;
      ch_tracer sim(device);
      sim.run([&](ch_tick t)->bool {
        std::cout << "t" << t << ": out="  << device.io.out << std::endl;
        return (t != 2*10);
      }, 2);

      ch_toVerilog("counter.v", device);
#if 0
      sim.toText("counter.log");
      sim.toVerilog("counter_tb.v", "counter.v");
      sim.toVCD("counter.vcd");  
      RetCheck ret;
      ret &= 0xc == device.io.out;
      return (0 == ret);
#endif
    });
  }
}

 
