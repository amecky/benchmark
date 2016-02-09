#include "BenchmarkRunner.h"
#include "Benchmark.h"
#include "src\Profiler.h"

BenchmarkRunner::BenchmarkRunner() {
}


BenchmarkRunner::~BenchmarkRunner() {
}

void BenchmarkRunner::add(Benchmark* benchmark, int runs, int iterations) {
	BenchmarkDefinition def;
	def.benchmark = benchmark;
	def.runs = runs;
	def.iterations = iterations;
	_benchmarks.push_back(def);
}

void BenchmarkRunner::execute() {
	for (size_t i = 0; i < _benchmarks.size(); ++i) {
		const BenchmarkDefinition& def = _benchmarks[i];
		float* timings = new float[def.runs];
		int cnt = 0;
		for (int r = 0; r < def.runs; ++r) {
			// start timing
			StopWatch sw(def.benchmark->getName());
			sw.start();			
			for (int j = 0; j < def.iterations; ++j) {
				def.benchmark->execute();
			}
			// stop timing
			sw.end();
			timings[cnt++] = sw.elapsed();
		}		
		BenchmarkResult result;
		result.minimum = timings[0];
		result.maximum = timings[0];
		result.elapsed = timings[0];
		for (int j = 1; j < def.runs; ++j) {
			if (timings[j] < result.minimum) {
				result.minimum = timings[j];
			}
			if (timings[j] > result.maximum) {
				result.maximum = timings[j];
			}
			result.elapsed += timings[j];
		}
		result.average = result.elapsed / static_cast<float>(def.runs);
		FILE* f = fopen("result.csv", "w");
		for (int j = 0; j < def.runs; ++j) {
			fprintf(f,"%g\n", timings[j]);
		}
		fclose(f);
		printf("===== Result =====\n");
		printf("Runs       : %d\n", def.runs);
		printf("Iterations : %d\n", def.iterations);
		printf("Total      : %g microsecs\n", result.elapsed);
		printf("Average    : %g microsecs\n", result.average);
		printf("Fastest    : %g microsecs\n", result.minimum);
		printf("Slowest    : %g microsecs\n", result.maximum);
		delete[] timings;
	}
}