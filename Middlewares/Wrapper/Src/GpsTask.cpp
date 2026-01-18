#include "GpsTask.hpp"

// GPS driver passed by reference (Dependency Injection)
GpsTask::GpsTask(IGps& gpsDriver) 
    : Thread("GPS_Task", 256, 2), // Stack: 256 words, Priority: 2 (Normal)
        _gps(gpsDriver) {}

void GpsTask::run(){
    _gps.init();

    while (1) {
        // Update the sensor (read & parse incoming data)
        if (_gps.update()) {
            // If there is new and valid data, notify other modules (LCD, Log)
            // GpsData data = _gps.getData();
            // You can enqueue the data using xQueueSend(...).
        }

        // GPS typically outputs at 1Hz (1000ms) or 10Hz.
        // Wait a bit to avoid tight looping, but ensure UART buffer doesn't overflow.
        // The best approach is to wake the task from a UART interrupt (semaphore).
        // For now, use polling:
        delay(100); 
    }
}
