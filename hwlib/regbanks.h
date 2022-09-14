#pragma once

#include "core.h"
#include "htl/decoupled.h"
#include "htl/mux.h"
#include "htl/enc.h"

namespace hwlib {

template <unsigned BankSize, // cache size (Bytes)
          unsigned RFBits>
          unsigned RFSize>
struct Config {
  static constexpr unsigned num_banks  = BankNums;
  static constexpr unsigned bank_bits  = RFBits * num_banks;
  static constexpr unsigned rf_data_bits  = RFBits;
  static constexpr unsigned rf_index_bits  = log2cail(RFSize);
  static constexpr unsigned rf_size  = RFSize;

  __interface (req_io, (
    __in (ch_bit<rf_index_bits)> addr,
    __in (ch_bool) read,
    __in (ch_bool) write,
    __in (ch_bit<rf_data_bits>) wdata,
    __out (ch_bit<rf_data_bits>) rdata
    __out (ch_bool)           readdatavalid,
    __out (ch_bool)           waitrequest
  ));

}

template <typename Cfg>
struct RegFile {
  __io (
    (typename Cfg::req_io) req
  );

private:

public:

  void describe() {
    ch_mem<rf_bits, Cfg::rf_size> rf(0); // zero initialized
  }
};

template <typename Cfg>
struct RegBanks {
  __io (
    ch_vec<(typename Cfg::req_io)> client_req
  );

  void describe() {
    ch_vec<ch_module<RegFile<Cfg>>, Cfg::num_banks> refbanks;
  }
};
}
