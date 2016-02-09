#include "Benchmark.h"
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

BENCHMARK(SimpleTest) {
	int i = 421234;
	//printf("Hello world %s\n",number_to_string(i).c_str());
	std::string str = number_to_string(i);
}
/*
class SimpleTest : public Benchmark {

public:
	SimpleTest() : Benchmark("SimpleTest") {}
	virtual void execute() {
		int i = 421234;
		//printf("Hello world %s\n",number_to_string(i).c_str());
		std::string str = number_to_string(i);
	}
};
*/