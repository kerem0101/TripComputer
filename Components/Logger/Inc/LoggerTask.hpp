#pragma once

#include "Thread.hpp"
#include "GpsData.h"
#include "IImu.hpp"
#include "cmsis_os.h"
#include "ff.h" // FatFs library

struct LogData {
    GpsData gps;
    ImuData imu;
};

class LoggerTask : public Thread {
public:
    LoggerTask();
    
    // Initialize RTOS objects (must be called after osKernelInitialize)
    void init();

    // Push data to the logger queue. Called from NavigationTask.
    bool pushData(const LogData& data);

protected:
    void run() override;

private:
    osMessageQueueId_t _logQueue;
    FATFS _fs;
};
