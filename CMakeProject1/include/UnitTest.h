#ifndef UNIT_TEST_H
#define UNIT_TEST_H

class UnitTest {
public:
    static void Initialize();
    static void RunTests();
    static bool AddTest(const char* testName, bool (*testFunction)());
    static void PrintResults();
    static int GetTestCount();
    static int GetPassedTests();
};

#endif // UNIT_TEST_H