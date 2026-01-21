#ifndef TELEMETRY_H
#define TELEMETRY_H

class Telemetry {
public:
    static void Initialize();
    static void SendEvent(const char* eventName, const char* data);
    static void RecordMetric(const char* metricName, double value);
    static void SetUserId(const char* userId);
    static void Flush();
};

#endif // TELEMETRY_H