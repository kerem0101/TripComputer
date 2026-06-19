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

// Debug variables to track task creation and heap usage via ST-Link Watch
volatile uint32_t dbg_heap_before = 0;
volatile uint32_t dbg_heap_after_logger = 0;
volatile uint32_t dbg_heap_after_all = 0;
volatile uint8_t dbg_logger_started = 0;
volatile uint8_t dbg_nav_started = 0;
volatile uint8_t dbg_display_started = 0;
volatile uint8_t dbg_system_started = 0;
volatile uint8_t dbg_queue_created = 0;

// ==========================================
// MAIN INITIALIZATION (Calling from main.c)
// ==========================================
void App_Main() {
    
    dbg_heap_before = xPortGetFreeHeapSize();
    
    // Initialize RTOS objects (must be called after osKernelInitialize in main.c)
    loggerTask.init();
    dbg_queue_created = (loggerTask.pushData(LogData{}) == false) ? 0 : 1; // Test if queue exists
    // Actually just check if init set the queue - pushData returns false if queue is NULL
    
    // Set the initial screen for the Trip Computer WITHOUT drawing it yet
    stateManager.setInitialState(&speedState);

    // Start LoggerTask FIRST - it needs the most heap
    dbg_logger_started = loggerTask.start() ? 1 : 0;
    dbg_heap_after_logger = xPortGetFreeHeapSize();
    
    dbg_nav_started = navTask.start() ? 1 : 0;
    dbg_display_started = displayTask.start() ? 1 : 0;
    dbg_system_started = systemTask.start() ? 1 : 0;
    dbg_heap_after_all = xPortGetFreeHeapSize();
}