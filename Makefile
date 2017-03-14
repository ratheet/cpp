CXX = g++


main : main.cpp
	g++ -fconcepts -O2 -std=c++1z main.cpp -o main

debug : main.cpp
	g++ -fconcepts -O0 -std=c++1z -g3 main.cpp -o debug

valgrind : debug
	valgrind -v --num-callers=20 --leak-check=yes --leak-resolution=high --show-reachable=yes ./debug

clean :
	rm -f *.o main
