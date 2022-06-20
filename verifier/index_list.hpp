#pragma once

#include "ternary_bitstream.hpp"

namespace resyn
{

class index_list
{
public:
  enum ntk_types
  {
    aig = 1,
    xag,
    mig,
    xmg,
    undef = 0
  };

public:
  index_list() {}
  index_list( uint32_t num_inputs, uint32_t num_outputs, uint32_t num_gates, ntk_types type = ntk_types::aig )
    : _num_inputs( num_inputs ), _num_outputs( num_outputs ), _num_gates( num_gates ), _type( type )
  {}

  void set_indices( std::vecotr<uint32_t> const& raw )
  {
    _indices = raw;
  }

  void simulate()
  {

  }

private:
  uint32_t _num_inputs, _num_outputs, _num_gates;
  ntk_types _type;
  std::vector<uint32_t> _indices;
  std::vector<ternary_bitstream> _sim;
};

} // namespace resyn