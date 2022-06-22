#pragma once

#include <vector>
#include <iostream>

namespace resyn
{

class ternary_bitstream
{
public:
  ternary_bitstream()
    : _num_bits( 0u )
  {}

  ternary_bitstream( uint64_t num_bits )
    : _num_bits( num_bits )
  {
    uint32_t num_blocks = ( num_bits & 0x3f ) ? ( num_bits >> 6 ) + 1 : num_bits >> 6;
    _mask.resize( num_blocks );
    _bits.resize( num_blocks );
  }

  uint64_t num_bits() const
  {
    return _num_bits;
  }

  bool get_bit( uint64_t index ) const
  {
    assert( index < _num_bits );
    return ( _bits.at( index >> 6 ) >> ( index & 0x3f ) ) & 0x1;
  }

  bool get_mask( uint64_t index ) const
  {
    assert( index < _num_bits );
    return ( _mask.at( index >> 6 ) >> ( index & 0x3f ) ) & 0x1;
  }

  void write_bit( uint64_t index, bool value )
  {
    assert( index < _num_bits );
    uint32_t ith_block = index >> 6;
    uint32_t index_in_block = index & 0x3f;
    _mask[ith_block] |= (uint64_t)1 << index_in_block;
    if ( value )
    {
      _bits[ith_block] |= (uint64_t)1 << index_in_block;
    }
  }

  void print( std::ostream& os = std::cout ) const
  {
    for ( uint64_t index = 0u; index < _num_bits; ++index )
    {
      if ( get_mask( index ) )
      {
        if ( get_bit( index ) )
          os << "1";
        else
          os << "0";
      }
      else
      {
        os << "-";
      }
    }
  }

  inline auto begin_bits() noexcept { return _bits.begin(); }
  inline auto end_bits() noexcept { return _bits.end(); }
  inline auto begin_bits() const noexcept { return _bits.begin(); }
  inline auto end_bits() const noexcept { return _bits.end(); }
  inline auto begin_mask() noexcept { return _mask.begin(); }
  inline auto end_mask() noexcept { return _mask.end(); }
  inline auto begin_mask() const noexcept { return _mask.begin(); }
  inline auto end_mask() const noexcept { return _mask.end(); }

public:
  uint64_t _num_bits;
  std::vector<uint64_t> _mask;
  std::vector<uint64_t> _bits;
}; /* ternary_bitstream */

template<typename Fn1, typename Fn2>
ternary_bitstream unary_operation( const ternary_bitstream& first, Fn1&& op_bits, Fn2&& op_mask )
{
  ternary_bitstream result( first.num_bits() );
  auto first_bits = first.begin_bits();
  auto first_mask = first.begin_mask();
  auto result_bits = result.begin_bits();
  auto result_mask = result.begin_mask();

  while ( first_bits != first.end_bits() )
  {
    *result_bits++ = op_bits( *first_bits, *first_mask );
    *result_mask++ = op_mask( *first_bits++, *first_mask++ );
  }

  return result;
}

template<typename Fn1, typename Fn2>
ternary_bitstream binary_operation( const ternary_bitstream& first, const ternary_bitstream& second, Fn1&& op_bits, Fn2&& op_mask )
{
  assert( first.num_bits() == second.num_bits() );

  ternary_bitstream result( first.num_bits() );
  auto first_bits = first.begin_bits();
  auto first_mask = first.begin_mask();
  auto second_bits = second.begin_bits();
  auto second_mask = second.begin_mask();
  auto result_bits = result.begin_bits();
  auto result_mask = result.begin_mask();

  while ( first_bits != first.end_bits() )
  {
    *result_bits++ = op_bits( *first_bits, *first_mask, *second_bits, *second_mask );
    *result_mask++ = op_mask( *first_bits++, *first_mask++, *second_bits++, *second_mask++ );
  }

  return result;
}

inline ternary_bitstream operator!( const ternary_bitstream& first )
{
  return unary_operation( first,
    []( auto bits1, auto mask1 ) { return ~bits1 & mask1; },
    []( auto bits1, auto mask1 ) { return mask1; } );
}

inline ternary_bitstream operator&( const ternary_bitstream& first, const ternary_bitstream& second )
{
  return binary_operation( first, second,
    []( auto bits1, auto mask1, auto bits2, auto mask2 ) { return bits1 & bits2; },
    []( auto bits1, auto mask1, auto bits2, auto mask2 ) { return  ( mask1 & mask2 ) | ( ~bits1 & mask1 ) | ( ~bits2 & mask2 ); } );
}

inline ternary_bitstream operator^( const ternary_bitstream& first, const ternary_bitstream& second )
{
  return binary_operation( first, second,
    []( auto bits1, auto mask1, auto bits2, auto mask2 ) { return ( bits1 ^ bits2 ) & ( mask1 & mask2 ); },
    []( auto bits1, auto mask1, auto bits2, auto mask2 ) { return mask1 & mask2; } );
}

} // namespace resyn