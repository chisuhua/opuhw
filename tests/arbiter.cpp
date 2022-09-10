#include "common.h"
/*
#include <htl/onehot.h>
#include <htl/mux.h>
#include <htl/xbar.h>
#include <htl/arbiter.h>
#include <htl/float32.h>
#include <htl/counter.h>
#include <htl/enc.h>
#include <htl/queue.h>
*/
#include <hwlib/arbiter.h>

using namespace ch::htl;

TEST_CASE("arbiter", "[arbiter]") {
  SECTION("meshArbiter", "[meshArbiter]") {
    TESTX([]()->bool {
      ch_device<meshArbiter<4, 4>> device;
      ch_tracer sim(device);
      ch_toVerilog("meshArbiter.v", device);
      sim.reset();
      device.io.h_in[0] = 0x0;
      device.io.h_in[1] = 0x0;
      device.io.h_in[2] = 0x0;
      device.io.h_in[3] = 0x0;
      sim.step(2);
      sim.reset();
#if 0
  sim.run([&](ch_tick t)->bool {
    // std::cout << "t" << t << ": ctr=" << device.io.out << std::endl;
    std::cout << "t" << t << ": grant=" << device.io.grant << std::endl;
    return (t != 2*10);
  }, 2);
#endif
      RetCheck ret;
      device.io.h_in[0] = 0x1;
      device.io.h_in[1] = 0x1;
      device.io.h_in[2] = 0x1;
      device.io.h_in[3] = 0x1;
      sim.step(1);
      ret &= 1 == device.io.grant[0];
      ret &= 0 == device.io.grant[1];
      ret &= 0 == device.io.grant[2];
      ret &= 0 == device.io.grant[3];
      std::cout << "-----------------------------------------------\n";
      device.io.h_in[0] = 0x1;
      device.io.h_in[1] = 0x1;
      device.io.h_in[2] = 0x1;
      device.io.h_in[3] = 0x1;
      sim.step(2);
      ret &= 2 == device.io.grant[0];
      ret &= 0 == device.io.grant[1];
      ret &= 0 == device.io.grant[2];
      ret &= 0 == device.io.grant[3];
      std::cout << "-----------------------------------------------\n";
      device.io.h_in[0] = 0x1;
      device.io.h_in[1] = 0x1;
      device.io.h_in[2] = 0x1;
      device.io.h_in[3] = 0x1;
      sim.step(2);
      ret &= 4 == device.io.grant[0];
      ret &= 0 == device.io.grant[1];
      ret &= 0 == device.io.grant[2];
      ret &= 0 == device.io.grant[3];
      std::cout << "-----------------------------------------------\n";
      device.io.h_in[0] = 0x1;
      device.io.h_in[1] = 0x1;
      device.io.h_in[2] = 0x1;
      device.io.h_in[3] = 0x1;
      sim.step(2);
      ret &= 8 == device.io.grant[0];
      ret &= 0 == device.io.grant[1];
      ret &= 0 == device.io.grant[2];
      ret &= 0 == device.io.grant[3];
      std::cout << "-----------------------------------------------\n";
      device.io.h_in[0] = 0x1;
      device.io.h_in[1] = 0x8;
      device.io.h_in[2] = 0x4;
      device.io.h_in[3] = 0x2;
      sim.step(2);
      ret &= 1 == device.io.grant[0];
      ret &= 8 == device.io.grant[1];
      ret &= 4 == device.io.grant[2];
      ret &= 2 == device.io.grant[3];
      std::cout << "-----------------------------------------------\n";
      device.io.h_in[0] = 0x1;
      device.io.h_in[1] = 0x2;
      device.io.h_in[2] = 0x4;
      device.io.h_in[3] = 0x8;
      sim.step(2);
      ret &= 1 == device.io.grant[0];
      ret &= 2 == device.io.grant[1];
      ret &= 4 == device.io.grant[2];
      ret &= 8 == device.io.grant[3];
      std::cout << "-----------------------------------------------\n";
      device.io.h_in[0] = 0x1;
      device.io.h_in[1] = 0x8;
      device.io.h_in[2] = 0x4;
      device.io.h_in[3] = 0x2;
      sim.step(2);
      ret &= 1 == device.io.grant[0];
      ret &= 8 == device.io.grant[1];
      ret &= 4 == device.io.grant[2];
      ret &= 2 == device.io.grant[3];
      std::cout << "-----------------------------------------------\n";
      device.io.h_in[0] = 0x1;
      device.io.h_in[1] = 0x8;
      device.io.h_in[2] = 0x4;
      device.io.h_in[3] = 0x2;
      sim.step(2);
      ret &= 1 == device.io.grant[0];
      ret &= 8 == device.io.grant[1];
      ret &= 4 == device.io.grant[2];
      ret &= 2 == device.io.grant[3];
      std::cout << "-----------------------------------------------\n";
      device.io.h_in[0] = 0x1;
      device.io.h_in[1] = 0x1;
      device.io.h_in[2] = 0x2;
      device.io.h_in[3] = 0x1;
      sim.step(2);
      ret &= 1 == device.io.grant[0];
      ret &= 4 == device.io.grant[1];
      ret &= 0 == device.io.grant[2];
      ret &= 0 == device.io.grant[3];
      std::cout << "-----------------------------------------------\n";
      device.io.h_in[0] = 0x1;
      device.io.h_in[1] = 0x1;
      device.io.h_in[2] = 0x0;
      device.io.h_in[3] = 0x1;
      sim.step(2);
      ret &= 2 == device.io.grant[0];
      ret &= 0 == device.io.grant[1];
      ret &= 0 == device.io.grant[2];
      ret &= 0 == device.io.grant[3];
      std::cout << "-----------------------------------------------\n";
      device.io.h_in[0] = 0x1;
      device.io.h_in[1] = 0x0;
      device.io.h_in[2] = 0x0;
      device.io.h_in[3] = 0x1;
      sim.step(2);
      ret &= 8 == device.io.grant[0];
      ret &= 0 == device.io.grant[1];
      ret &= 0 == device.io.grant[2];
      ret &= 0 == device.io.grant[3];
      std::cout << "-----------------------------------------------\n";
      device.io.h_in[0] = 0x1;
      device.io.h_in[1] = 0x1;
      device.io.h_in[2] = 0x1;
      device.io.h_in[3] = 0x0;
      sim.step(2);
      ret &= 1 == device.io.grant[0];
      ret &= 0 == device.io.grant[1];
      ret &= 0 == device.io.grant[2];
      ret &= 0 == device.io.grant[3];
      std::cout << "-----------------------------------------------\n";
      device.io.h_in[0] = 0x0;
      device.io.h_in[1] = 0x0;
      device.io.h_in[2] = 0x1;
      device.io.h_in[3] = 0x1;
      sim.step(2);
      ret &= 4 == device.io.grant[0];
      ret &= 0 == device.io.grant[1];
      ret &= 0 == device.io.grant[2];
      ret &= 0 == device.io.grant[3];
      std::cout << "finished --------------------------------------\n";
      sim.toVCD("meshArbiter.vcd");
      sim.toText("meshArbiter.log");
      return !!ret;
    });
  }
}
