#include "common.h"
#include <htl/onehot.h>
#include <htl/mux.h>
#include <htl/xbar.h>
#include <htl/arbiter.h>
#include <htl/float32.h>
#include <htl/counter.h>
#include <htl/enc.h>
#include <htl/queue.h>
#include <hwlib/arbiter.h>

using namespace ch::htl;

TEST_CASE("arbiter", "[arbiter]") {
  SECTION("matArbiter", "[matArbiter]") {
    TESTX([]()->bool {
      ch_device<ch_matArbiter<4>> device;
      // ch_simulator sim(device);
      ch_tracer sim(device);
      sim.reset();
      device.io.in[0] = false;
      device.io.in[1] = false;
      device.io.in[2] = false;
      device.io.in[3] = false;
      sim.step(2);
      RetCheck ret;
      ret &= 0 == device.io.grant;
      device.io.in[1] = true;
      sim.step(2);
      ret &= 2 == device.io.grant;
      device.io.in[2] = true;
      sim.step(4);
      ret &= 4 == device.io.grant;
      device.io.in[3] = true;
      sim.step(6);
      ret &= 8 == device.io.grant;
      device.io.in[1] = false;
      device.io.in[2] = false;
      sim.step(2);
      ret &= 8 == device.io.grant;
      device.io.in[3] = false;
      sim.step(2);
      ret &= 0 == device.io.grant;
      device.io.in[0] = true;
      sim.step(2);
      ret &= 1 == device.io.grant;
      sim.toVCD("matAribiter1.vcd");
      return !!ret;
    });
    TESTX([]()->bool {
      RetCheck ret;
      ch_device<ch_matArbiter<5>> device;
      // ch_simulator sim(device);
      ch_tracer sim(device);
      sim.reset();
      device.io.in = 0x1f;  
      sim.step(2);
      ret &= 0x8 == device.io.grant;
      sim.step(2);
      device.io.in = 0xf;
      ret &= 0x4 == device.io.grant;
      sim.step(2);
      ret &= 0x2 == device.io.grant;
      sim.step(2);
      ret &= 0x1 == device.io.grant;
      sim.step(2);
      ret &= 0x8 == device.io.grant;
      sim.toVCD("matAribiter2.vcd");
      return !!ret;
    });
  }

  SECTION("switch", "[switch]") {
    TESTX([]()->bool {
      ch_device<ch_xbar_switch<ch_bit4, 2>> device;
      ch_simulator sim(device);
      sim.reset();
      device.io.in[0].data  = 0xA;
      device.io.in[0].valid = false;
      device.io.in[1].data  = 0xB;
      device.io.in[1].valid = false;
      device.io.out.ready   = false;
      sim.step(2);
      RetCheck ret;
      ret &= !device.io.out.valid;
      ret &= !device.io.in[0].ready;
      ret &= !device.io.in[1].ready;
      device.io.out.ready = true;
      sim.step(2);
      ret &= !device.io.out.valid;
      ret &= !device.io.in[0].ready;
      ret &= !device.io.in[1].ready;
      device.io.in[0].valid = true;
      sim.step(2);
      ret &= !!device.io.out.valid;
      ret &= !!device.io.in[0].ready;
      ret &= !device.io.in[1].ready;
      ret &= 0xA == device.io.out.data;
      device.io.in[1].valid = true;
      sim.step(4);
      ret &= !!device.io.out.valid;
      ret &= !device.io.in[0].ready;
      ret &= !!device.io.in[1].ready;
      ret &= 0xB == device.io.out.data;
      return !!ret;
    });
  }
}
