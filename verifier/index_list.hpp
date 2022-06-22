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

  void set_indices( std::vector<uint32_t> const& raw )
  {
    assert( raw.size() == _num_gates * num_fanins() + _num_outputs );
    _indices = raw;
  }

  void simulate( std::vector<ternary_bitstream> const& input_tts )
  {
    assert( input_tts.size() == _num_inputs );
    uint32_t num_bits = input_tts[0].num_bits();
    ternary_bitstream const0( num_bits );

    _sim.clear();
    _sim.reserve( _num_inputs + _num_gates + 1 );
    _sim.emplace_back( const0 );

    for ( auto const& tt : input_tts )
    {
      _sim.emplace_back( tt );
    }

    uint32_t num_indices_gates = _num_gates * num_fanins();
    for ( auto i = 0u; i < num_indices_gates; i += num_fanins() )
    {
      _sim.emplace_back( compute( i ) );
    }
  }

  std::vector<ternary_bitstream> output_tts() const
  {
    assert( _sim.size() == _num_inputs + _num_gates + 1 );
    uint32_t num_before = _num_gates * num_fanins();
    std::vector<ternary_bitstream> tts;
    tts.reserve( _num_outputs );
    for ( auto i = 0u; i < _num_outputs; ++i )
    {
      uint32_t o = _indices.at( num_before + i );
      tts.emplace_back( o & 0x1 ? !_sim.at( o >> 1 ) : _sim.at( o >> 1) );
    }
    return tts;
  }

  inline uint32_t num_fanins() const
  {
    switch ( _type )
    {
      case aig:
      case xag:
        return 2;
      case mig:
      case xmg:
        return 3;
      default:
        assert( false );
        return 0;
    }
  }

  inline ternary_bitstream compute( uint32_t i ) const
  {
    switch ( _type )
    {
      case aig:
        return compute_and( _indices.at( i ), _indices.at( i + 1 ) );
      case xag:
        if ( _indices.at( i ) < _indices.at( i + 1 ) )
          return compute_and( _indices.at( i ), _indices.at( i + 1 ) );
        else
          return compute_xor( _indices.at( i ), _indices.at( i + 1 ) );
      case mig:
        //return compute_maj( _indices.at( i ), _indices.at( i + 1 ), _indices.at( i + 2 ) );
      case xmg:
      default:
        assert( false );
        return ternary_bitstream();
    }
  }

  inline ternary_bitstream compute_and( uint32_t id1, uint32_t id2 ) const
  {
    assert( id1 >> 1 < _sim.size() && id2 >> 1 < _sim.size() );
    return ( id1 & 0x1 ? !_sim.at( id1 >> 1 ) : _sim.at( id1 >> 1 ) ) & ( id2 & 0x1 ? !_sim.at( id2 >> 1 ) : _sim.at( id2 >> 1 ) );
  }

  inline ternary_bitstream compute_xor( uint32_t id1, uint32_t id2 ) const
  {
    assert( id1 >> 1 < _sim.size() && id2 >> 1 < _sim.size() );
    return ( id1 & 0x1 ? !_sim.at( id1 >> 1 ) : _sim.at( id1 >> 1 ) ) ^ ( id2 & 0x1 ? !_sim.at( id2 >> 1 ) : _sim.at( id2 >> 1 ) );
  }

private:
  uint32_t _num_inputs, _num_outputs, _num_gates;
  ntk_types _type;
  std::vector<uint32_t> _indices;
  std::vector<ternary_bitstream> _sim;
};

} // namespace resyn