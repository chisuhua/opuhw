#pragma once

#include "core.h"
#include "htl/decoupled.h"
#include "htl/mux.h"
#include "htl/enc.h"
#include "regbanks.h"

namespace hwlib {

template <typename Cfg>
struct Opc {
  __io (
    (typename Cfg::req_io) req
  );

private:

public:

  void describe() {
  }
};
}
