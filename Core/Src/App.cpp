#include "App.h"
#include "main.h" // for HAL library access
#include "bsp_board.h" // for BSP functions
#include "SystemTask.hpp" // for SystemTask class
#include "Neo6M.hpp" // for Neo6M GPS driver
#include "GpsTask.hpp" // for GpsTask class
#include "LcdParallel.hpp" // for LcdParallel display driver
#include "DisplayTask.hpp" // for DisplayTask class

static Neo6M gpsDriver;
static GpsTask gpsTask(gpsDriver);
static LcdParallel lcdDriver;
static DisplayTask displayTask(lcdDriver, gpsDriver);

static SystemTask systemTask;

void App_Main() {
    
    bool gpsStatus = gpsTask.start();
    if (!gpsStatus)
        Error_Handler();

    bool displayStatus = displayTask.start();
    if (!displayStatus)
        Error_Handler();

    bool status = systemTask.start();
    if (!status)
        Error_Handler(); 

}