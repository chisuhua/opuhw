#pragma once

#include "core.h"
#include "htl/decoupled.h"
#include "htl/counter.h"
#include "htl/xbar.h"
#include "htl/onehot.h"

using namespace ch::htl;
using namespace ch::logic;

template <unsigned N>
struct counter {
  __io (
    __out (ch_uint<N>) out
  );

  void describe() {    
    ch_reg<ch_uint<N>> out(2);
    out->next = out + 1;
    io.out = out;
  }
};

