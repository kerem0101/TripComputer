#pragma once

#include "Thread.hpp"
#include "IDisplay.hpp"
#include "GpsData.h" 
#include "IGps.hpp"

class DisplayTask : public Thread {
public:
    // DisplayTask need both display and GPS interfaces, so we inject them via constructor
    DisplayTask(IDisplay& display, IGps& gps);

protected:
    void run() override;

private:
    IDisplay& _display;
    IGps& _gps;
};