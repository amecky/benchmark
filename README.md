# benchmark
Measuring time is an important step when trying to optimize a piece of software. This tool is a simple benchmarking utility written in simple C++. It is 
a single header C++ implementation and therefore easy to integrate and use. Just put the BenchmarkRunner.h in your project.

## Concept

The tool provides a simple Macro to run your benchmarks. Basically it looks like this:

	BENCHMARK("SomeName",runs, iterations) {
		// run the code to measure
	}
There is a differentation between runs and iterations. Probably your code will be too fast to do some proper measuring and therefore you can define the number of iterations for a run. The time will be measured for the number of iterations. The number of runs defines how many times the code will be tested. For every run the time will be measured and saved. At the end you will get a simple result telling you the total time and the average, slowest and fatest time of every run.

## How to build

Just take the BenchmarkRunner.h and include them in your project. At one point you need to define BENCHMARK_IMPLEMENTATION.

Then in your main method you simply call:
#define BENCHMARK_IMPLEMENTATION
#include ".BenchmarkRunner.h"

    int main(int argc, char* argv[]) {
	    BenchmarkRunner::getInstance()->execute();
	    return 0;
    }

## How to use it
In order to run a benchmark you need to use the BENCHMARK macro. It will take a name of the test and two parameters. The first one is the number of runs and the second one the number of iterations. 

## Example
The following is a simple way of measuring the time it takes to convert an integer into a string. The method shown here is definitely not the best way to implement this. 

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
Of course you can add as many Benchmarks as you like to your code. But beware that the name of the benchmark must be unique.

## The output
The following shows the output of such a benchmark. 

    =========== Result ===========
    Benchmark  : SimpleTest
    Runs       : 20
    Iterations : 10
    Total      : 5511.92 microsecs
    Average    : 275.596 microsecs
    Fastest    : 264.427 microsecs
    Slowest    : 325.402 microsecs

## Some notes
This small tool currenly only runs under windows. I might change it to use std::chrono to make it portable. 

## License
It is released under the MIT license

## Contact
Just send an email to amecky@gmail.com

