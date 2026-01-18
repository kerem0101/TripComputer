#pragma once
#include "GpsData.h" // GpsData struct definition

// Interface class (abstract base)
class IGps {
public:
    virtual ~IGps() {}

    // Initialize the hardware (UART, etc.)
    virtual void init() = 0;

    // Check for new incoming data and parse it.
    // Typically called periodically from an RTOS task.
    virtual bool update() = 0;

    // Return the most recent valid data
    virtual GpsData getData() const = 0;
};