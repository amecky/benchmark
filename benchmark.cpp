#include "BenchmarkRunner.h"
#include "SimpleTest.h"

int main(int argc, char* argv[]) {
	gBenchmarkRunner = new BenchmarkRunner;
	gBenchmarkRunner->execute();
	//runner.add(new SimpleTest(),20,100);
	//runner.execute();
	return 0;
}

