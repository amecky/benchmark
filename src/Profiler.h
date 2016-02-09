#pragma once
#define PROFILING TRUE
#include <windows.h>
#include <chrono>

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

#define TIMER(name) StopWatch s(name); s.start();
/*
// -------------------------------------------------------
// Profile data
// -------------------------------------------------------
struct ProfileSnapshot {

	char name[32];
	int level;
	int invokeCounter;
	float totalTime;

};

namespace perf {

	void init();

	void reset();

	void finalize();

	int start(const char* name);

	void end(int index);

	void shutdown();

	void debug();

	void save(const ReportWriter& writer);

	//void showDialog(v2* position);

	//int get_snapshot(ProfileSnapshot* items, int max);

	float get_current_total_time();
}

class ZoneTracker {

public:
	ZoneTracker(const char* name) {
		_index = perf::start(name);
	}
	~ZoneTracker() {
		perf::end(_index);
	}
private:
	int _index;
};

// -------------------------------------------------------
// profiler
// -------------------------------------------------------
namespace profiler {

	void init();

	void reset();

	void start(const char* name);

	void end(const char* name);

	void finalize();

	void print();

	void save(FILE* f);

	void save(const ReportWriter& writer);

	int get_total_times(float* values, int max);

	void showDialog(v2* position);

	int get_snapshot(ProfileSnapshot* items, int max);

	float get_current_total_time();
}

//#define PRS(name) do { profiler::start(name); } while(0)
//#define PRE(name) do { profiler::end(name); } while(0)

//#ifdef PROFILING
//#define PR_START(a) do { profiler::start(a); } while(0)
//#define PR_END(a) do { profiler::end(a); } while(0)
//#else
//#define PR_START(a) 
//#define PR_END(a) 
//#endif

*/