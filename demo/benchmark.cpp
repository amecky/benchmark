#define BENCHMARK_IMPLEMENTATION
#include "..\source\BenchmarkRunner.h"
#include "SimpleTest.h"

int main(int argc, char* argv[]) {
	CSVFileResultWriter csvWriter;
	BenchmarkRunner::getInstance()->addResultWriter(csvWriter);
	BenchmarkRunner::getInstance()->execute();
	return 0;
}

