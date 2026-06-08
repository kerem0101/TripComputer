#include "App.h"
#include "main.h" // for HAL library access
#include "bsp_board.h" // for BSP functions
#include "SystemTask.hpp" // for SystemTask class
#include "Neo6M.hpp" // for Neo6M GPS driver
#include "NavigationTask.hpp" // for NavigationTask class
#include "LcdParallel.hpp" // for LcdParallel display driver
#include "DisplayTask.hpp" // for DisplayTask class
#include "Button.hpp"
#include "SpeedState.hpp"
#include "TripState.hpp"
#include "StateManager.hpp"
#include "Mpu6050.hpp" // for Mpu6050 IMU driver
#include "SensorFusion.hpp" // for SensorFusion class

// ==========================================
// 1. DRIVERS 
// ==========================================
static Neo6M gpsDriver;
static LcdParallel lcdDriver;
static Button mainButton(BTN_ID_MAIN);
static Mpu6050 imuDriver;
static SensorFusion sensorFusion(gpsDriver, imuDriver);

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
// StateManager needs to know how to draw (lcdDriver) and where to get data (fused gps/imu data)
static StateManager stateManager(lcdDriver, sensorFusion);

// ==========================================
// 4. RTOS TASKS 
// ==========================================
static NavigationTask navTask(sensorFusion);
static DisplayTask displayTask(stateManager, mainButton, lcdDriver);
static SystemTask systemTask;

// ==========================================
// MAIN INITIALIZATION (Calling from main.c)
// ==========================================
void App_Main() {
    
    //Set the initial screen for the Trip Computer
    stateManager.changeState(&speedState);

    navTask.start();
    displayTask.start();
    systemTask.start();
}