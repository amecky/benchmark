
class Benchmark {

public:
	Benchmark(const char* name) : _name(name) {}
	virtual ~Benchmark() {}
	virtual void execute() = 0;
	const char* getName() const {
		return _name;
	}
private:
	Benchmark() {}
	Benchmark(const Benchmark& other) {}
	const char* _name;
};

#define BENCHMARK_CLASS_NAME(benchmark_name) \
    benchmark_name ## _Benchmark

#define BENCHMARK(NAME) \
	class NAME : public Benchmark { \
	public: \
		NAME() : Benchmark("Test") {\
			BenchmarkRunner.getInstance()->add(this,10,10); \
		} \
		virtual ~NAME() {} \
		virtual void execute(); \
	}; \
	void NAME::execute()