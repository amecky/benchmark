# benchmark
Very simple benchmarking utility written in simple C++. Easy to integrate and use.
## How to build
Just take the BenchmarkRunner.h and BenchmarkRunner.cpp files located in the source subdirectory and include them in your project.
Then in your main method you simply call:

    int main(int argc, char* argv[]) {
	    BenchmarkRunner::getInstance()->execute();
	    return 0;
    }

## How to use it
In order to run a benchmark you need to use the BENCHMARK macro. It will take a name of the test and two parameters. The first one is the number of runs and the second one the number of iterations. The time measurement will be taken for the number of iterations. For example if your method is too fast to be measured you can increase the number of iterations. Every run will then call your method multiply by iterations. But only the time of every run is measured.

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
