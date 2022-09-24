#include "common.h"
#include <hwlib/regbank.h>

using namespace ch::htl;
using namespace hwlib;

TEST_CASE("regbank", "[regbank]") {
  SECTION("basic", "[basic]") {
    TESTX([]()->bool {
      using Cfg = RFConfig<4, 32, 128>;
      ch_device<RegBank<Cfg>> device;
      RetCheck ret;

      ch_tracer sim(device);
      ch_toVerilog("regbank.v", device);
      sim.reset();
      for (unsigned i = 0; i < 4; ++i) {
        device.io.req[i].addr = i;
        device.io.req[i].read = 0x0;
        device.io.req[i].write = 0x1;
        device.io.req[i].valid = true;
        device.io.req[i].wdata = i*5;
      }
      sim.step(2);
      // read addr step 1
      for (unsigned i = 0; i < 4; ++i) {
        device.io.req[i].addr = i;
        device.io.req[i].read = 0x1;
        device.io.req[i].write = 0x0;
        device.io.req[i].valid = true;
      }
      sim.step(2);
      // read addr step 2
      for (unsigned i = 0; i < 4; ++i) {
        device.io.req[i].addr = i*4;
        device.io.req[i].read = 0x1;
        device.io.req[i].write = 0x0;
        device.io.req[i].valid = true;
      }
      sim.step(2);
      // write addr step 2
      for (unsigned i = 0; i < 4; ++i) {
        device.io.req[i].addr = i*4;
        device.io.req[i].read = 0x0;
        device.io.req[i].write = 0x1;
        device.io.req[i].valid = true;
        device.io.req[i].wdata = i*0xc;
      }
      sim.step(2);
      // read addr step 2
      for (unsigned i = 0; i < 4; ++i) {
        device.io.req[i].addr = i*4;
        device.io.req[i].read = 0x1;
        device.io.req[i].write = 0x0;
        device.io.req[i].valid = true;
      }
      sim.step(2);
      // read addr step 1
      for (unsigned i = 0; i < 4; ++i) {
        device.io.req[i].addr = i;
        device.io.req[i].read = 0x1;
        device.io.req[i].write = 0x0;
        device.io.req[i].valid = true;
      }
      sim.step(2);
      sim.toVCD("regbank.vcd");
      return ret;
    });
  }
}
