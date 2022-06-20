#pragma once

#include "ternary_bitstream.hpp"

namespace resyn
{

class resynthesis_problem
{
public:
  resynthesis_problem() {}

  resynthesis_problem( uint32_t I, uint32_t N, uint32_t T, uint32_t L )
    : _I( I ), _N( N ), _T( T ), _L( L ), _divs( I + N, ternary_bitstream( L ) ), _spec( 1u << T )
  {}

  void set_divisor( uint32_t index, std::string const& str )
  {
    assert( index < _divs.size() );
    for ( auto it = str.begin(), auto i = 0u; it != str.end(); it++, i++ )
    {
      if ( *it == '1' || *it == '0' )
        _divs[index].write_bit( i, *it == '1' );
    }
  }

  void set_specification( uint32_t index, std::string const& str )
  {
    assert( index < _spec.size() );
    for ( auto it = str.begin(), auto i = 0u; it != str.end(); it++, i++ )
    {
      
    }
  }

private:
  uint32_t _I, _N, _T, _L;
  std::vector<ternary_bitstream> _divs;
  std::vector<> _spec;
};

} // namespace resyn