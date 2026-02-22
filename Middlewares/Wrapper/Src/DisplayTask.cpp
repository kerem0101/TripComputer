#include "DisplayTask.hpp"
#include <cstdio> // for sprintf 

DisplayTask::DisplayTask(IDisplay& display, IGps& gps) 
    : Thread("DisplayTask", 256, 2), // Stack: 256, Priority: Normal
      _display(display), 
      _gps(gps) {
}

void DisplayTask::run() {
    _display.init();
    _display.clear();
    _display.print(0, 0, "Trip Computer");
    _display.print(1, 0, "Starting...");
    
    delay(2000); // Startup message for 2 seconds
    _display.clear();

    char lineBuffer[17]; // 16 char + null terminator

    while (1) {
        // 1. Read GPS data (we will use Queue for real implementation, but for simplicity, we read directly here)
        GpsData data = _gps.getData();

        // 2. Write to display
        if (data.isValid) {
            // 1. Row: Speed
            // "Speed: 12.5 km"
            sprintf(lineBuffer, "Speed: %d km/h    ", (int)data.speedKmh);
            _display.print(0, 0, lineBuffer);

            // 2. Row: Satellites or Coordinates
            // "Sat: 5  Lat:41"
            sprintf(lineBuffer, "Sat:%d           ", data.satellites);
            _display.print(1, 0, lineBuffer);
        } else {
            _display.print(0, 0, "GPS Searching...");
            _display.print(1, 0, "No Signal       ");
        }

        // Display update every 500 ms
        delay(500); 
    }
}