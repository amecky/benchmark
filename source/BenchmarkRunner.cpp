#include "BenchmarkRunner.h"

double LIToMicroSecs(LARGE_INTEGER & L, LARGE_INTEGER & F) {
	return ((double)L.QuadPart * 1000.0 * 1000.0 / (double)F.QuadPart);
}

// ---------------------------------------------
// benchmark runner instance
// ---------------------------------------------
BenchmarkRunner* BenchmarkRunner::_runner = 0;

// ---------------------------------------------
// add
// ---------------------------------------------
void BenchmarkRunner::add(BenchmarkFunctionPtr benchmark, const char* name, int runs, int iterations) {
	Benchmark def;
	def.benchmark = benchmark;
	def.runs = runs;
	def.name = name;
	def.iterations = iterations;
	_benchmarks.push_back(def);
}

// ---------------------------------------------
// execute
// ---------------------------------------------
void BenchmarkRunner::execute() {
	LARGE_INTEGER frequency;
	LARGE_INTEGER startingTime;
	LARGE_INTEGER EndingTime;
	QueryPerformanceFrequency(&frequency);
	for (size_t i = 0; i < _benchmarks.size(); ++i) {
		Benchmark& def = _benchmarks[i];
		def.timings = new float[def.runs];
		int cnt = 0;
		for (int r = 0; r < def.runs; ++r) {
			QueryPerformanceCounter(&startingTime);
			for (int j = 0; j < def.iterations; ++j) {
				(*def.benchmark)();
			}
			QueryPerformanceCounter(&EndingTime);
			LARGE_INTEGER time;
			time.QuadPart = EndingTime.QuadPart - startingTime.QuadPart;
			def.timings[cnt++] = LIToMicroSecs(time, frequency);
		}		
		def.minimum = def.timings[0];
		def.maximum = def.timings[0];
		def.elapsed = def.timings[0];
		for (int j = 1; j < def.runs; ++j) {
			if (def.timings[j] < def.minimum) {
				def.minimum = def.timings[j];
			}
			if (def.timings[j] > def.maximum) {
				def.maximum = def.timings[j];
			}
			def.elapsed += def.timings[j];
		}
		def.average = def.elapsed / static_cast<float>(def.runs);
		// generate output
		for (size_t i = 0; i < _writers.size(); ++i) {
			_writers[i]->generate(def);
		}
	}
}