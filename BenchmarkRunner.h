#pragma once
#include <vector>

class Benchmark;

struct BenchmarkResult {
	int interations;
	float minimum;
	float maximum;
	float average;
	float elapsed;
};

class BenchmarkRunner {

	struct BenchmarkDefinition {
		Benchmark* benchmark;
		int runs;
		int iterations;

		BenchmarkDefinition() : benchmark(0), runs(1), iterations(1) {}
	};

	typedef std::vector<BenchmarkDefinition> Benchmarks;

public:
	BenchmarkRunner();
	~BenchmarkRunner();
	void add(Benchmark* benchmark, int runs, int iterations);
	void execute();
	static BenchmarkRunner* getInstance() {
		if (_runner == 0) {
			_runner = new BenchmarkRunner();
		}
		return _runner;
	}
private:
	Benchmarks _benchmarks;
	static BenchmarkRunner*_runner;
};


