#include "verifier.hpp"

#include <iostream>

int main( int argc, char* argv[] )
{
  using namespace resyn;

  if ( argc == 2 )
  {
    std::cout << "[i] Parse resyn + solution file `" << argv[1] << "` and verify...\n";
    bool const res = parse_and_verify( argv[1] );
    if ( res )
    {
      std::cout << "[i] Solution is valid.\n";
    }
    else
    {
      std::cout << "[i] Solution is invalid.\n";
    }
  }
  else if ( argc == 3 )
  {
    std::cout << "[i] Parse resyn file `" << argv[1] << "` and solution file `" << argv[2] << "` and verify...\n";
    bool const res = parse_and_verify( argv[1], argv[2] );
    if ( res )
    {
      std::cout << "[i] Solution is valid.\n";
    }
    else
    {
      std::cout << "[i] Solution is invalid.\n";
    }
  }
  else
  {
    std::cout << "[i] Please pass one or two filenames as arguments.\n";
    std::cout << "    Usage:\n";
    std::cout << "    ./verify <resyn+sol>   (problem and solution in the same file)\n";
    std::cout << "    ./verify <resyn> <sol> (problem in the first file and solution in the second file)\n";
  }

  return 0;
}