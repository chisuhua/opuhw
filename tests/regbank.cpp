#include "common.h"
#include <hwlib/regbank.h>

using namespace hwlib;

TEST_CASE("regbank", "[regbank]") {
  SECTION("basic", "[basic]") {
    TESTX([]()->bool {
      using Cfg = Config<512, 8, 4, 20, 32, 8>;
      ch_device<regbank<Cfg>> regbank;
      RetCheck ret;

      ch_tracer tracer(regbank);
      for (unsigned i = 0; i < 4, ++i) {
        regbank.io.addr = 0;
      }
      return ret;
    });
  }
}
