#pragma once

#include "FreeRTOS.h"
#include "task.h"

// Interface (Abstract Class)
class Thread {
public:
    // Constructor: Preapare task parameters
    Thread(const char* name, uint16_t stackDepth, UBaseType_t priority)
        : _name(name), _stackDepth(stackDepth), _priority(priority), _taskHandle(nullptr) {}

    virtual ~Thread() {
        // When task is deleted cleanup may require 
    }

    // Save task the scheduler and start it
    bool start() {
        BaseType_t result = xTaskCreate(
            staticTaskFunction, // Tramboline function
            _name,
            _stackDepth,
            this,               // "this" pointer to pass to static function
            _priority,
            &_taskHandle
        );
        return (result == pdPASS);
    }

    // Task delay ms
    void delay(uint32_t ms) {
        vTaskDelay(pdMS_TO_TICKS(ms));
    }

protected:
    // virtual function to be implemented by derived classes
    virtual void run() = 0;

private:
    // STATIC TRAMPOLINE
    // FreeRTOS C function waits for a C++ member function, but cannot call it directly.
    // This static function serves as an intermediary.
    static void staticTaskFunction(void* pvParameters) {
        // void* parameters points to the instance of the Thread class
        Thread* thread = static_cast<Thread*>(pvParameters);
        
        // Call the member function
        if (thread) {
            thread->run();
        }
        
        // If the run function exits, delete the task
        vTaskDelete(NULL);
    }

    const char* _name;
    uint16_t _stackDepth;
    UBaseType_t _priority;
    TaskHandle_t _taskHandle;
};