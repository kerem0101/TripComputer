#pragma once
#include "Thread.hpp"
#include "IGps.hpp"
#include "IImu.hpp"
#include "LoggerTask.hpp"

class NavigationTask : public Thread {
public:
    // Constructor (Dependency Injection)
    NavigationTask(IGps& gpsDriver, IImu& imuDriver, LoggerTask* logger = nullptr);

protected:
    void run() override;

private:
    IGps& _gps;
    IImu& _imu;
    LoggerTask* _logger;
};