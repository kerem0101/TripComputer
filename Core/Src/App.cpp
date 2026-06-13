#include "App.h"
#include "main.h" // for HAL library access
#include "bsp_board.h" // for BSP functions
#include "SystemTask.hpp" // for SystemTask class
#ifdef SIMULATION_MODE
#include "MockGps.hpp"
#include "MockImu.hpp"
#else
#include "Neo6M.hpp" // for Neo6M GPS driver
#include "Mpu6050.hpp" // for Mpu6050 IMU driver
#endif
#include "NavigationTask.hpp" // for NavigationTask class
#include "LcdParallel.hpp" // for LcdParallel display driver
#include "DisplayTask.hpp" // for DisplayTask class
#include "Button.hpp"
#include "SpeedState.hpp"
#include "TripState.hpp"
#include "StateManager.hpp"
#include "SensorFusion.hpp" // for SensorFusion class
#include "LoggerTask.hpp" // for LoggerTask class

// ==========================================
// 1. DRIVERS 
// ==========================================
#ifdef SIMULATION_MODE
static MockGps gpsDriver;
static MockImu imuDriver;
#else
static Neo6M gpsDriver;
static Mpu6050 imuDriver;
#endif

static LcdParallel lcdDriver;
static Button mainButton(BTN_ID_MAIN);
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
static LoggerTask loggerTask;
static NavigationTask navTask(sensorFusion, imuDriver, &loggerTask);
static DisplayTask displayTask(stateManager, mainButton, lcdDriver);
static SystemTask systemTask;

// ==========================================
// MAIN INITIALIZATION (Calling from main.c)
// ==========================================
void App_Main() {
    
    // Initialize RTOS objects (must be called after osKernelInitialize in main.c)
    loggerTask.init();

    // Set the initial screen for the Trip Computer WITHOUT drawing it yet
    stateManager.setInitialState(&speedState);

    navTask.start();
    displayTask.start();
    systemTask.start();
    loggerTask.start();
}