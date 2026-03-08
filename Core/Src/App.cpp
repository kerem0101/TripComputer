#include "App.h"
#include "main.h" // for HAL library access
#include "bsp_board.h" // for BSP functions
#include "SystemTask.hpp" // for SystemTask class
#include "Neo6M.hpp" // for Neo6M GPS driver
#include "GpsTask.hpp" // for GpsTask class
#include "LcdParallel.hpp" // for LcdParallel display driver
#include "DisplayTask.hpp" // for DisplayTask class
#include "Button.hpp"
#include "SpeedState.hpp"
#include "TripState.hpp"
#include "StateManager.hpp"

// ==========================================
// 1. DRIVERS 
// ==========================================
static Neo6M gpsDriver;
static LcdParallel lcdDriver;
static Button mainButton(BTN_ID_MAIN);

// ==========================================
// 2. UI STATES
// ==========================================
static SpeedState speedState;
static TripState tripState;

// Global pointers used by the states to transition to each other
IState* speedStateInstance = &speedState;
IState* tripStateInstance = &tripState;

// ==========================================
// 3. MANAGERS (Yöneticiler)
// ==========================================
// StateManager needs to know how to draw (lcdDriver) and where to get data (gpsDriver)
static StateManager stateManager(lcdDriver, gpsDriver);

// ==========================================
// 4. RTOS TASKS 
// ==========================================
static GpsTask gpsTask(gpsDriver);
static DisplayTask displayTask(stateManager, mainButton, lcdDriver);
static SystemTask systemTask;

// ==========================================
// MAIN INITIALIZATION (Calling from main.c)
// ==========================================
void App_Main() {
    
    //Set the initial screen for the Trip Computer
    stateManager.changeState(&speedState);

    gpsTask.start();
    displayTask.start();
    systemTask.start();
}