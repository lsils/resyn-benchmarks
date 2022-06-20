#pragma once

#include "resynthesis_problem.hpp"
#include "index_list.hpp"

#include <sstream>
#include <iostream>
#include <string>
#include <vector>

namespace resyn
{

inline void split( std::string const& str, std::vector<std::string>& strings, char delim = ' ' )
{
  strings.clear();
  std::istringstream f( str );
  std::string element;

  while ( std::getline( f, element, delim ) )
  {
    strings.emplace_back( element );
  }
}

inline resynthesis_problem read_problem( std::istream& in )
{
  uint32_t I, N, T{0}, L;

  std::string line;
  std::vector<std::string> strings;
  while ( std::getline( in, line ) )
  {
    split( line, strings );
    if ( strings.size() != 5 )
      continue;
    if ( strings[0] == "resyn" )
    {
      I = std::stoi( strings[1] );
      N = std::stoi( strings[2] );
      T = std::stoi( strings[3] );
      L = std::stoi( strings[4] );
      break;
    }
  }

  assert( T != 0 );
  resynthesis_problem prob( I, N, T, L );
  for ( auto i = 0u; i < I + N; ++i )
  {
    std::getline( in, line );
    assert( line.size() == L );
    prob.set_divisor( i, line );
  }
  for ( auto i = 0u; i < (1u << T); ++i )
  {
    std::getline( in, line );
    assert( line.size() == L );
    prob.set_specification( i, line );
  }
}

inline index_list read_solution( std::istream& in, resynthesis_problem const& prob )
{
  std::string name;
  index_list::ntk_types type = index_list::ntk_types::undef;
  uint32_t K;

  std::string line;
  std::vector<std::string> strings;
  while ( std::getline( in, line ) )
  {
    split( line, strings );
    if ( strings.size() != 4 )
      continue;
    if ( strings[0] == "sol" )
    {
      name = strings[1];
      if ( strings[2] == "aig" )
        type = index_list::ntk_types::aig;
      K = std::stoi( strings[3] );
      break;
    }
  }

  assert( type != index_list::ntk_types::undef );
  index_list res( prob.num_divs(), prob.num_targets(), K, type );
  std::getline( in, line );
  split( line, strings );
  std::vector<uint32_t> indices( strings.size() );
  std::transform( strings.begin(), string.end(), indices.begin(), []( std::string str ) { return std::stoi( str ); } );
  res.set_indices( indices );
  return res;
}

} // namespace resyn