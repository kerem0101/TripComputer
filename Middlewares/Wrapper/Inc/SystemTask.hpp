#pragma once
#include "Thread.hpp"

class SystemTask : public Thread {
public:
    // System task constructor
    SystemTask();

protected:
    // Override the run method
    void run() override;
};