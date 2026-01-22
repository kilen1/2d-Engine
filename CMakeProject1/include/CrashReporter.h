#ifndef CRASH_REPORTER_H
#define CRASH_REPORTER_H

class CrashReporter {
public:
    static void Initialize();
    static void ReportCrash(const char* message);
    static void SetCrashHandler(void (*handler)(int));
};

#endif // CRASH_REPORTER_H