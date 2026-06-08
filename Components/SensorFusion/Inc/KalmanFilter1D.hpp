#pragma once

class KalmanFilter1D {
public:
    KalmanFilter1D();

    // Initialize the filter with initial velocity and acceleration bias
    void init(float initial_v, float initial_bias);

    // Predict step: called frequently (e.g., 50Hz) using IMU acceleration
    // dt: time step in seconds
    // accel: forward acceleration in m/s^2
    void predict(float accel, float dt);

    // Update step: called when new GPS data is available (e.g., 1Hz)
    // measured_v: speed measured by GPS in m/s
    void update(float measured_v);

    // Get the current estimated velocity in m/s
    float getVelocity() const;

private:
    float v;       // Estimated velocity
    float bias;    // Estimated acceleration bias
    
    // Covariance matrix elements for [v, bias]
    float P00, P01, P10, P11;

    // Process noise variances
    float Q_accel; // Variance of acceleration noise
    float Q_bias;  // Variance of bias drift

    // Measurement noise variance
    float R_meas;  // Variance of GPS speed measurement
};
