#ifndef BENCHMARK_H
#define BENCHMARK_H

class Benchmark {
public:
    static void Initialize();
    static void StartBenchmark(const char* testName);
    static void EndBenchmark(const char* testName);
    static void RunAllBenchmarks();
    static void PrintResults();
};

#endif // BENCHMARK_H