#pragma once
#include "IGps.hpp"
#include "GpsData.h" // GpsData struct definition

class Neo6M : public IGps {
public:
    // Constructor
    Neo6M();
    // IGps interface implementations
    void init() override;
    bool update() override;
    GpsData getData() const override;
private:
    GpsData _data;
};