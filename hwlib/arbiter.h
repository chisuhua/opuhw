#pragma once

#include "core.h"
#include "htl/decoupled.h"
#include "htl/counter.h"
#include "htl/xbar.h"
#include "htl/onehot.h"

using namespace ch::htl;
using namespace ch::logic;

// x is client num
// y is service num
template <unsigned xN, unsigned yN>
struct meshArbiter {
  __io (
    __in (ch_vec<ch_bit<yN>, xN>)  h_in,
    __out (ch_vec<ch_bit<xN>, yN>) grant
    // __out (ch_uint<log2ceil(yN)>) out
  );

  void describe() {
    const int yNlog2 = log2ceil(yN);
    int xx[xN];
    for (unsigned i = 0; i < xN; ++i) {
      xx[i] = (yN - i) % yN;
    }
    // x ---->
    // y | x0y0 x1y3 x2y2 x3y1
    //   | x0y1 x1y0 x2y3 x3y2
    //   | x0y2 x1y1 x2y0 x3y3
    //   | x0y3 x1y2 x2y1 x3y0
    //   | x0y0 x1y3 x2y2 x3y1
    //
    ch_reg<ch_uint<yNlog2>> rr_ctr(0);

    ch_bool active(true);
    // io.out = rr_ctr;
#if 1
    ch_vec<ch_vec<ch_uint<log2ceil(yN)>, xN>, yN> rry;

    for (unsigned y = 0; y < yN; ++y) {
      for (unsigned x = 0; x < xN; ++x) {
        rry[y][x] = rr_ctr + xx[x] + y;
      }
    }

    ch_vec<ch_vec<ch_bool, xN>, yN> ena(false);
    ch_vec<ch_vec<ch_bool, xN>, yN> dis_x(false);
    ch_vec<ch_vec<ch_bool, xN>, yN> dis_y(false);

    for (unsigned y = 0; y < yN; ++y) {
      for (unsigned x = 0; x < xN; ++x) {
        ena[y][x] = ((io.h_in[x] >> rry[y][x]) == true) & (~dis_x[y][x]) & (~dis_y[y][x]);
        ch_cout << "rry[" << y << "][" << x << "]=" <<  rry[y][x] << ", io.h_in[" << x << "]=" << io.h_in[x] << ", ena[" << y << "][" << x << "]=" << ena[y][x] << std::endl;
      }
      if (y > 0) {
        for (unsigned x = 0; x < xN; ++x) {
          int x_left;
          if (x == 0) {
            x_left = xN - 1;
          } else {
            x_left = x - 1;
          }

          dis_x[y][x] = ena[y-1][x] or dis_x[y-1][x];
          dis_y[y][x] = ena[y-1][x_left] or dis_y[y-1][x_left];
        }
      }
    }

    ch_vec<ch_bit<yN>, xN>   grant(false);
    for (unsigned x = 0; x < xN; ++x) {
      for (unsigned y = 0; y < yN; ++y) {
        __if (ena[y][x]) {
          grant[x] = (ch_bit<yN>(1) << rry[y][x]);
          active = true;
        };
      }
    }

    for (unsigned y = 0; y < xN; ++y) {
      for (unsigned x = 0; x < xN; ++x) {
        io.grant[y][x] = grant[x][y];
        ch_cout << " io.grant[" << y << "][" << x << "]=" << io.grant[y][x] << std::endl;
      }
    }

    __if (active) {
      rr_ctr->next = rr_ctr + 1;
    };
    ch_cout << "clk=" << ch_now() << " active=" << active << " rr_ctr=" << rr_ctr << std::endl;
#endif
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

