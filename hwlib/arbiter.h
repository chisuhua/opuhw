#pragma once

#include "../core.h"
#include "decoupled.h"
#include "counter.h"
#include "xbar.h"
#include "onehot.h"

namespace ch {
namespace htl {

using namespace ch::logic;

template <unsigned N>
struct ch_meshArbiter {
  __io (
    __in (ch_vec<ch_bit<N>, N>)  hot_in,
    __out (ch_vec<ch_bit<N>, N>) grant
  );

  void describe() {
    ch_vec<ch_vec<ch_int<2>, N>, N> _inmatch(false);
    ch_vec<ch_vec<ch_bool, N>, N> _request(false);
    ch_reg<ch_int<N>>  _pri;

    for (int i = 0; i < N; ++i) {
      for (int j = 0; j < N; j++) {
        _request[i][j] = false;
      }
      if (ch_orr(hot_in[i])) {
        auto oc_id = ch_hot2bin(hot_in[i]);
        _request[i][oc_id] = 1;
      }
    }

    for (int p = 0; p < N; ++p) {
      output = (_pri + p) % _outputs;

      // Step through the current diagonal
      for (input = 0; input < N; ++input) {
        if ((output < N) && (_inmatch[input] == false) &&
          (_request[input][output])) {
          _inmatch[input] = output;
        }
        output = (output + 1) % _outputs;
      }
    }

    _pri = (_pri + 1) % _outputs;

    for (unsigned i = 0; i < N; i++) {
      auto output = _inmatch[i];
      io.grant[i][j] = output;
    }
  }
};

/*
template <typename T, unsigned I, typename Arbiter = ch_matArbiter<I>>
struct ch_xbar_switch {
  using value_type   = T;
  using arbiter_type = Arbiter;
  using in_io        = ch_vec<ch_enq_io<T>, I>;
  using out_io       = ch_deq_io<T>;
    
  __io (
    (in_io)           in,
    (out_io)          out,
    __out (ch_bit<I>) grant
  );

  void describe() {
    ch_module<ch_hxbar<ch_valid_t<T>, I, 1>> xbar;
    ch_module<Arbiter> arb;

    for (unsigned i = 0; i < I; ++i) {
      xbar.io.in[i].data  = io.in[i].data;
      xbar.io.in[i].valid = io.in[i].valid;
      arb.io.in[i]        = io.in[i].valid;
    }

    io.out.data  = xbar.io.out[0].data;
    io.out.valid = xbar.io.out[0].valid;
    xbar.io.sel  = arb.io.grant;

    for (unsigned i = 0; i < I; ++i) {
      io.in[i].ready = io.out.ready && arb.io.grant[i];
    }

    io.grant = arb.io.grant;
  }  
};
*/

}
}
