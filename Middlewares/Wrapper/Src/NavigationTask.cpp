#include "NavigationTask.hpp"

// Navigation driver passed by reference (Dependency Injection)
NavigationTask::NavigationTask(IGps& gpsDriver) 
    : Thread("Nav_Task", 256, 2), // Stack: 256 words, Priority: 2 (Normal)
        _gps(gpsDriver) {}

void NavigationTask::run(){
    _gps.init();

    while (1) {
        // Update the sensor (this will internally read IMU at high freq and GPS at low freq)
        if (_gps.update()) {
            // If there is new and valid data, notify other modules (LCD, Log)
            // GpsData data = _gps.getData();
            // You can enqueue the data using xQueueSend(...).
        }

        // Run at 50Hz (20ms delay) to keep up with IMU updates
        delay(20); 
    }
}
