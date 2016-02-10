#include "BenchmarkRunner.h"
#include <stdio.h>


std::string number_to_string(int x){
	if (!x) return "0";
	std::string s, s2;
	while (x){
		s.push_back(x % 10 + '0');
		x /= 10;
	}
	std::reverse(s.begin(), s.end());
	return s;
}

BENCHMARK("SimpleTest", 20, 10) {
	int i = 421234;
	std::string str = number_to_string(i);
}


BENCHMARK("SimpleTest2", 20, 10) {
	int i = 42;
	std::string str = number_to_string(i);
}
