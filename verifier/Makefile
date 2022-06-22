CC := g++
CFLAGS := -std=c++14 -g
exe = verify

all:main.cpp index_list.hpp parsers.hpp resynthesis_problem.hpp ternary_bitstream.hpp verifier.hpp
	@$(CC) main.cpp -o $(exe) $(CFLAGS)

clean:
	@rm -f *.o $(exe)