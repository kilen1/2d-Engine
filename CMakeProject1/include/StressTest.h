#ifndef STRESS_TEST_H
#define STRESS_TEST_H

class StressTest {
public:
    static void Initialize();
    static void StartStressTest(const char* testName, int durationSeconds);
    static void StopStressTest();
    static void RunAllStressTests();
    static void SetMaxLoad(int maxLoad);
};

#endif // STRESS_TEST_H