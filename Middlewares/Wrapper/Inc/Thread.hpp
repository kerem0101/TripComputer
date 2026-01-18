#pragma once

#include "FreeRTOS.h"
#include "task.h"

// Interface (Abstract Class)
class Thread {
public:
    // Constructor: Preapare task parameters
    Thread(const char* name, uint16_t stackDepth, UBaseType_t priority);

    virtual ~Thread(); // When task is deleted cleanup may require 

    // Save task the scheduler and start it
    bool start();

    // Task delay ms
    void delay(uint32_t ms);

protected:
    // virtual function to be implemented by derived classes
    virtual void run() = 0;

private:
    // STATIC TRAMPOLINE
    // FreeRTOS C function waits for a C++ member function, but cannot call it directly.
    // This static function serves as an intermediary.
    static void staticTaskFunction(void* pvParameters);

    const char* _name;
    uint16_t _stackDepth;
    UBaseType_t _priority;
    TaskHandle_t _taskHandle;
};