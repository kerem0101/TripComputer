#include "Thread.hpp"

// Constructor: Preapare task parameters
Thread::Thread(const char* name, uint16_t stackDepth, UBaseType_t priority)
    : _name(name), _stackDepth(stackDepth), _priority(priority), _taskHandle(nullptr) {}

Thread::~Thread() {
    // When task is deleted cleanup may require 
}

// Save task the scheduler and start it
bool Thread::start() {
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
void Thread::delay(uint32_t ms) {
    vTaskDelay(pdMS_TO_TICKS(ms));
}

// STATIC TRAMPOLINE
// FreeRTOS C function waits for a C++ member function, but cannot call it directly.
// This static function serves as an intermediary.
void Thread::staticTaskFunction(void* pvParameters) {
    // void* parameters points to the instance of the Thread class
    Thread* thread = static_cast<Thread*>(pvParameters);
    
    // Call the member function
    if (thread) {
        thread->run();
    }
    
    // If the run function exits, delete the task
    vTaskDelete(NULL);
}