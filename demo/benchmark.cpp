#include "..\source\BenchmarkRunner.h"
#include "SimpleTest.h"

int main(int argc, char* argv[]) {
	BenchmarkRunner::getInstance()->execute();
	return 0;
}

