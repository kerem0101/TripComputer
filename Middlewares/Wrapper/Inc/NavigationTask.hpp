#pragma once
#include "Thread.hpp"
#include "IGps.hpp"

class NavigationTask : public Thread {
public:
    // Constructor (Dependency Injection)
    NavigationTask(IGps& gpsDriver);

protected:
    void run() override;

private:
    IGps& _gps;
};