#include "BenchmarkRunner.h"

double LIToMicroSecs(LARGE_INTEGER & L, LARGE_INTEGER & F) {
	return ((double)L.QuadPart * 1000.0 * 1000.0 / (double)F.QuadPart);
}

StopWatch::StopWatch() {
	_running = false;
	sprintf_s(_name, 32, "StopWatch");
	QueryPerformanceFrequency(&frequency);
}

StopWatch::StopWatch(const char* name) {
	_running = false;
	sprintf_s(_name, 32, name);
	QueryPerformanceFrequency(&frequency);
}

StopWatch::~StopWatch() {
	if (_running) {
		end();
	}
}

void StopWatch::start() {
	QueryPerformanceCounter(&startingTime);
	_running = true;
}

void StopWatch::end() {
	_running = false;
	LARGE_INTEGER EndingTime;
	QueryPerformanceCounter(&EndingTime);
	LARGE_INTEGER time;
	time.QuadPart = EndingTime.QuadPart - startingTime.QuadPart;
	_delta = LIToMicroSecs(time, frequency);
}

double StopWatch::elapsed() {
	return _delta;
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
	char buffer[128];
	for (size_t i = 0; i < _benchmarks.size(); ++i) {
		const Benchmark& def = _benchmarks[i];
		float* timings = new float[def.runs];
		int cnt = 0;
		for (int r = 0; r < def.runs; ++r) {
			// start timing
			StopWatch sw(def.name);
			sw.start();			
			for (int j = 0; j < def.iterations; ++j) {
				(*def.benchmark)();
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
		sprintf_s(buffer, 128, "%s_result.csv", def.name);
		FILE* f = fopen(buffer, "w");
		for (int j = 0; j < def.runs; ++j) {
			fprintf(f, "%d;%g\n", j, timings[j]);
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