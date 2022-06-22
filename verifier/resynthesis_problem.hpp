#pragma once

#include "ternary_bitstream.hpp"

#include <vector>
#include <set>
#include <string>

namespace resyn
{

class resynthesis_problem
{
public:
  resynthesis_problem() {}

  resynthesis_problem( uint32_t I, uint32_t N, uint32_t T, uint32_t L )
    : _I( I ), _N( N ), _T( T ), _L( L ), _divs( I + N, ternary_bitstream( L ) ), _spec( L )
  {}

  void set_divisor( uint32_t index, std::string const& str )
  {
    assert( index < _divs.size() );
    assert( str.length() == _L );
    auto i = 0u;
    for ( auto it = str.begin(); it != str.end(); it++, i++ )
    {
      if ( *it == '1' || *it == '0' )
        _divs[index].write_bit( i, *it == '1' );
    }
  }

  void set_specification( uint32_t pattern, std::string const& str )
  {
    assert( pattern < ( 1 << _T ) );
    assert( str.length() == _spec.size() );
    auto i = 0u;
    for ( auto it = str.begin(); it != str.end(); it++, i++ )
    {
      if ( *it == '1' )
      {
        _spec[i].insert( pattern );
      }
      else
      {
        assert( *it == '0' );
      }
    }
  }

  bool check( std::vector<ternary_bitstream> const& tts ) const
  {
    assert( tts.size() == _T );
    
    std::vector<uint32_t> to_check;
    for ( auto index = 0u; index < _L; ++index )
    {
      to_check.clear();
      to_check.emplace_back( 0u );
      for ( auto o = 0u; o < _T; ++o )
      {
        if ( tts[o].get_mask( index ) )
        {
          if ( tts[o].get_bit( index ) )
          {
            for ( uint32_t& pattern : to_check )
              pattern |= 1u << o;
          }
        }
        else
        {
          uint32_t current_size = to_check.size();
          for ( auto i = 0u; i < current_size; ++i )
          {
            to_check.emplace_back( to_check[i] );
            to_check.back() |= 1u << o;
          }
        }
      }

      for ( auto const& pattern : to_check )
      {
        if ( _spec[index].find( pattern ) == _spec[index].end() )
        {
          std::cout << "[i] Violated at the " << index << "-th bit (counted from 0 from the left):\n";
          std::cout << "    Accepted target patterns (in decimal) are ";
          for ( auto const& accepted : _spec[index] )
            std::cout << accepted << " ";
          std::cout << "\n    Simulated target functions:";
          for ( auto o = 0u; o < _T; ++o )
          {
            tts[o].print();
            std::cout << " (" << index << "-th bit = " <<
              ( tts[o].get_mask( index ) ? ( tts[o].get_bit( index ) ? "1" : "0" ) : "-" ) << ")\n";
          }
          return false;
        }
      }
    }
    return true;
  }

  uint32_t num_divs() const
  {
    return _I + _N;
  }

  uint32_t num_targets() const
  {
    return _T;
  }

public:
  uint32_t _I, _N, _T, _L;
  std::vector<ternary_bitstream> _divs;
  std::vector<std::set<uint32_t>> _spec; // for each minterm, a set of accepted PO patterns
};

} // namespace resyn