#pragma once
#include "Thread.hpp"
#include "IGps.hpp"

class GpsTask : public Thread {
public:
    // Constructor (Dependency Injection)
    GpsTask(IGps& gpsDriver);

protected:
    void run() override;

private:
    IGps& _gps;
};