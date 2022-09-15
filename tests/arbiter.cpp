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
using namespace hwlib;

TEST_CASE("arbiter", "[arbiter]") {
  SECTION("mh_arbiter", "[mh_arbiter]") {
    TESTX([]()->bool {
      ch_device<mh_arbiter<4, 4>> device;
      ch_tracer sim(device);
      ch_toVerilog("mh_arbiter.v", device);
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
      sim.toVCD("mh_arbiter.vcd");
      sim.toText("mh_arbiter.log");
      return !!ret;
    });
  }
  SECTION("mh_xbar", "[mh_xbar]") {
    TESTX([]()->bool {
      ch_device<mh_xbar<ch_bit4, 2, 2>> device;
      ch_tracer sim(device);
      ch_toVerilog("mh_xbar.v", device);
      RetCheck ret;
      sim.reset();
      device.io.in[0].data = 0xA;
      device.io.in[0].valid = true;
      device.io.h_sel[0] = 0x1;
      device.io.in[1].data = 0xB;
      device.io.in[1].valid = true;
      device.io.h_sel[1] = 0x2;
      device.io.out[0].ready = true;
      device.io.out[1].ready = false;
      sim.step(2);
      ret &= 0xa == device.io.out[0].data;
      ret &= 0xb == device.io.out[1].data;
      ret &= true == device.io.in[0].ready;
      ret &= false == device.io.in[1].ready;
      device.io.in[0].data = 0xC;
      device.io.in[0].valid = true;
      device.io.h_sel[0] = 0x2;
      device.io.in[1].data = 0xD;
      device.io.in[1].valid = true;
      device.io.h_sel[1] = 0x1;
      device.io.out[0].ready = true;
      device.io.out[1].ready = false;
      sim.step(2);
      ret &= 0xd == device.io.out[0].data;
      ret &= 0xc == device.io.out[1].data;
      ret &= false == device.io.in[0].ready;
      ret &= true == device.io.in[1].ready;
      std::cout << "finished --------------------------------------\n";
      sim.toVCD("mh_xbar.vcd");
      sim.toText("mh_xbar.log");
      return !!ret;
    });
  }
}
