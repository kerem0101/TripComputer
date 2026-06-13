#pragma once

#include "IGps.hpp"
#include "IImu.hpp"
#include "KalmanFilter1D.hpp"

class SensorFusion : public IGps {
public:
    // Takes references to the raw GPS and IMU drivers
    SensorFusion(IGps& gps, IImu& imu);

    // IGps interface implementation
    void init() override;
    bool update() override;
    GpsData getData() const override;
    void resetTripDistance() override;

private:
    IGps& _gps;
    IImu& _imu;
    
    KalmanFilter1D _kf;
    GpsData _fusedData;
    
    // To track time delta for IMU integration
    uint32_t _lastUpdateMs;
};
