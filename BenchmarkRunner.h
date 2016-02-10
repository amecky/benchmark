#pragma once
#include <windows.h>
#include <vector>

// ---------------------------------------------
// Stopwatch
// ---------------------------------------------
class StopWatch {

public:
	StopWatch();
	explicit StopWatch(const char* name);
	~StopWatch();
	void start();
	void end();
	double elapsed();

private:
	char _name[32];
	bool _running;
	LARGE_INTEGER startingTime;
	LARGE_INTEGER frequency;
	float _delta;
};

#define INTERNAL_UNIQUE_NAME( name ) name##__FILE__##__LINE__ 

// ---------------------------------------------
// benchmark function pointer
// ---------------------------------------------
typedef void(*BenchmarkFunctionPtr)(void);


struct AutoReg;

// ---------------------------------------------
// define macro
// ---------------------------------------------
#define BENCHMARK(Name,Runs,Iterations) \
        static void INTERNAL_UNIQUE_NAME( ____BENCHMARK____ )(); \
        namespace{ AutoReg INTERNAL_UNIQUE_NAME( autoRegistrar )( &INTERNAL_UNIQUE_NAME(  ____BENCHMARK____ ),Name,Runs,Iterations); }\
        static void INTERNAL_UNIQUE_NAME(  ____BENCHMARK____ )()

// ---------------------------------------------
// Benchmark result
// ---------------------------------------------
struct BenchmarkResult {
	int interations;
	float minimum;
	float maximum;
	float average;
	float elapsed;
};

// ---------------------------------------------
// Benchmark runner
// ---------------------------------------------
class BenchmarkRunner {

	struct Benchmark {
		const char* name;
		BenchmarkFunctionPtr benchmark;
		int runs;
		int iterations;

		Benchmark() : benchmark(0), runs(1), iterations(1) {}
	};

	typedef std::vector<Benchmark> Benchmarks;

public:
	void add(BenchmarkFunctionPtr benchmark,const char* name, int runs, int iterations);
	void execute();
	static BenchmarkRunner* getInstance() {
		if (_runner == 0) {
			_runner = new BenchmarkRunner();
		}
		return _runner;
	}
private:
	BenchmarkRunner() {}
	BenchmarkRunner(const BenchmarkRunner& other) {}
	~BenchmarkRunner() {}
	Benchmarks _benchmarks;
	static BenchmarkRunner*_runner;
};


// ---------------------------------------------
// AutoReg
// ---------------------------------------------
struct AutoReg {

	AutoReg(BenchmarkFunctionPtr ptr,const char* name,int runs,int iterations) {
		BenchmarkRunner::getInstance()->add(ptr, name, runs, iterations);
	}

	~AutoReg() {}

private:
	AutoReg(AutoReg const&);
	void operator= (AutoReg const&);
};

