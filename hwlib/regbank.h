#pragma once

#include "core.h"
#include "htl/decoupled.h"
#include "htl/mux.h"
#include "htl/enc.h"
#include "hwlib/arbiter.h"

using namespace ch::logic;

namespace hwlib {

template <unsigned BankNums, // number of banks
          unsigned RFBits,
          unsigned RFSize>
struct RFConfig {
  static constexpr unsigned num_banks  = BankNums;
  static constexpr unsigned bank_bits  = RFBits * num_banks;
  static constexpr unsigned rf_data_bits  = RFBits;
  static constexpr unsigned rf_index_bits  = log2ceil(RFSize);
  static constexpr unsigned rf_size  = RFSize;

  __interface (req_io, (
    __in (ch_bit<rf_index_bits>) addr,
    __in (ch_bool) valid,
    __in (ch_bool) read,
    __in (ch_bool) write,
    __in (ch_bit<rf_data_bits>) wdata,
    __out (ch_bit<rf_data_bits>) rdata
  ));
};

template <typename Cfg>
struct RegFile {
  __io (
    (typename Cfg::req_io) req
  );

private:
  using data_t = ch_bit<Cfg::rf_data_bits>;

public:

  void describe() {
    ch_mem<data_t, Cfg::rf_size> rf(0); // zero initialized
    ch_bool rd_en = io.req.valid && io.req.read;
    ch_bool wr_en = io.req.valid && io.req.write;
    io.req.rdata = ch_sel(rd_en, rf.read(io.req.addr), 0);
    rf.write(io.req.addr, io.req.wdata, wr_en);
  }
};

template <typename Cfg>
class RegBank {
public:
  using req_io_t = typename Cfg::req_io;

  __io (
      (ch_vec<req_io_t, Cfg::num_banks>) req
  );

  void describe() {
    ch_vec<ch_module<RegFile<Cfg>>, Cfg::num_banks> regbank;
    for (unsigned i = 0; i < Cfg::num_banks; ++i) {
      regbank[i].io.req.addr = io.req[i].addr;
      regbank[i].io.req.valid = io.req[i].valid;
      regbank[i].io.req.read = io.req[i].read;
      regbank[i].io.req.write = io.req[i].write;
      regbank[i].io.req.wdata = io.req[i].wdata;
      io.req[i].rdata = regbank[i].io.req.rdata;
    }
  }
};
}
