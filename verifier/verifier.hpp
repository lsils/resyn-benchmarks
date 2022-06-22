#pragma once

#include "parsers.hpp"
#include "resynthesis_problem.hpp"
#include "index_list.hpp"

#include <iostream>
#include <fstream>

namespace resyn
{

inline bool verify( resynthesis_problem const& prob, index_list& sol )
{
  sol.simulate( prob._divs );
  bool valid = prob.check( sol.output_tts() );
  return valid;
}

inline bool parse_and_verify( std::string const& filename )
{
  std::ifstream in( filename, std::ifstream::in );
  if ( !in.is_open() )
  {
    std::cerr << "[e] Cannot open file " << filename << "\n";
    return false;
  }
  else
  {
    resynthesis_problem const prob = read_problem( in );
    index_list sol = read_solution( in, prob );
    in.close();
    return verify( prob, sol );
  }
}

inline bool parse_and_verify( std::string const& filename_resyn, std::string const& filename_sol )
{
  std::ifstream in_resyn( filename_resyn, std::ifstream::in );
  std::ifstream in_sol( filename_sol, std::ifstream::in );
  if ( !in_resyn.is_open() )
  {
    std::cerr << "[e] Cannot open file " << filename_resyn << "\n";
    return false;
  }
  else if ( !in_sol.is_open() )
  {
    std::cerr << "[e] Cannot open file " << filename_sol << "\n";
    return false;
  }
  else
  {
    resynthesis_problem const prob = read_problem( in_resyn );
    index_list sol = read_solution( in_sol, prob );
    in_resyn.close();
    in_sol.close();
    return verify( prob, sol );
  }
}

} // namespace resyn