#pragma once
#include <windows.h>
#include <vector>

#define INTERNAL_UNIQUE_NAME_ALL( name , line ) name##line
#define INTERNAL_UNIQUE_NAME2( name, line ) INTERNAL_UNIQUE_NAME_ALL(name,line)
#define INTERNAL_UNIQUE_NAME( name ) INTERNAL_UNIQUE_NAME2(name,__LINE__)

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


#ifdef BENCHMARK_IMPLEMENTATION
// ---------------------------------------------
// Benchmark
// ---------------------------------------------
struct Benchmark {
	const char* name;
	BenchmarkFunctionPtr benchmark;
	int runs;
	int iterations;
	float minimum;
	float maximum;
	float average;
	float elapsed;
	float* timings;

	Benchmark() : benchmark(0), runs(1), iterations(1) , minimum(0.0f) , maximum(0.0f) , average(0.0f) , elapsed(0.0f) , timings(0) {}

	~Benchmark() {
		if (timings != 0) {
			delete[] timings;
		}
	}
};
// ---------------------------------------------
// Result writer base class
// ---------------------------------------------
class ResultWriter {

public:
	ResultWriter() {}
	virtual ~ResultWriter() {}
	virtual void generate(const Benchmark& benchmark) const = 0;
};

// ---------------------------------------------
// Console result writer
// ---------------------------------------------
class ConsoleResultWriter : public ResultWriter {

public:
	ConsoleResultWriter() : ResultWriter() {}
	virtual ~ConsoleResultWriter() {}
	void generate(const Benchmark& benchmark) const {
		printf("=========== Result ===========\n");
		printf("Benchmark  : %s\n", benchmark.name);
		printf("Runs       : %d\n", benchmark.runs);
		printf("Iterations : %d\n", benchmark.iterations);
		printf("Total      : %g microsecs\n", benchmark.elapsed);
		printf("Average    : %g microsecs\n", benchmark.average);
		printf("Fastest    : %g microsecs\n", benchmark.minimum);
		printf("Slowest    : %g microsecs\n", benchmark.maximum);
	}
};

// ---------------------------------------------
// CSV file result writer
// ---------------------------------------------
class CSVFileResultWriter : public ResultWriter {

public:
	CSVFileResultWriter() : ResultWriter() {}
	virtual ~CSVFileResultWriter() {}
	void generate(const Benchmark& benchmark) const {
		char buffer[128];
		sprintf_s(buffer, 128, "%s_result.csv", benchmark.name);
		FILE* f = fopen(buffer, "w");
		for (int j = 0; j < benchmark.runs; ++j) {
			fprintf(f, "%g\n", benchmark.timings[j]);
		}
		fclose(f);
	}
};

double LIToMicroSecs(LARGE_INTEGER & L, LARGE_INTEGER & F) {
	return ((double)L.QuadPart * 1000.0 * 1000.0 / (double)F.QuadPart);
}
// ---------------------------------------------
// Benchmark runner
// ---------------------------------------------
class BenchmarkRunner {

typedef std::vector<Benchmark> Benchmarks;
typedef std::vector<ResultWriter*> ResultWriters;

public:
	void addResultWriter(ResultWriter& writer) {
		_writers.push_back(&writer);
	}
	void add(BenchmarkFunctionPtr benchmark, const char* name, int runs, int iterations) {
		Benchmark def;
		def.benchmark = benchmark;
		def.runs = runs;
		def.name = name;
		def.iterations = iterations;
		_benchmarks.push_back(def);
	}
	void execute() {
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
	static BenchmarkRunner* getInstance() {
		if (_runner == 0) {
			_runner = new BenchmarkRunner();			
		}
		return _runner;
	}
private:
	BenchmarkRunner() {
		_writers.push_back(&_consoleWriter);
	}
	BenchmarkRunner(const BenchmarkRunner& other) {}
	~BenchmarkRunner() {}
	Benchmarks _benchmarks;
	ResultWriters _writers;
	ConsoleResultWriter _consoleWriter;
	static BenchmarkRunner*_runner;
};

// ---------------------------------------------
// benchmark runner instance
// ---------------------------------------------
BenchmarkRunner* BenchmarkRunner::_runner = 0;
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

#endif