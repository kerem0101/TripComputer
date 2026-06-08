#include "KalmanFilter1D.hpp"

KalmanFilter1D::KalmanFilter1D() {
    init(0.0f, 0.0f);
    
    // Tuning parameters (these can be adjusted based on testing)
    Q_accel = 0.01f; // Trust the IMU acceleration this much
    Q_bias = 0.001f; // Trust that bias doesn't change fast
    R_meas = 2.0f;   // Trust the GPS speed this much (lower means more trust)
}

void KalmanFilter1D::init(float initial_v, float initial_bias) {
    v = initial_v;
    bias = initial_bias;
    
    P00 = 1.0f;
    P01 = 0.0f;
    P10 = 0.0f;
    P11 = 1.0f;
}

void KalmanFilter1D::predict(float accel, float dt) {
    // 1. Project the state ahead
    // v = v + (accel - bias) * dt
    v += (accel - bias) * dt;
    // bias = bias (constant bias model)

    // 2. Project the error covariance ahead
    // P = F * P * F^T + Q
    // F = [1, -dt]
    //     [0,   1]
    
    P00 += dt * (dt * P11 - P01 - P10) + Q_accel;
    P01 -= dt * P11;
    P10 -= dt * P11;
    P11 += Q_bias;
}

void KalmanFilter1D::update(float measured_v) {
    // 1. Compute Kalman gain
    // K = P * H^T * (H * P * H^T + R)^-1
    // H = [1, 0] (we only measure velocity)
    
    // S = H * P * H^T + R = P00 + R_meas
    float S = P00 + R_meas;
    
    // K = [P00 / S]
    //     [P10 / S]
    float K0 = P00 / S;
    float K1 = P10 / S;
    
    // 2. Update state with measurement
    // y = z - H * x (innovation)
    float y = measured_v - v;
    
    v += K0 * y;
    bias += K1 * y;
    
    // 3. Update error covariance
    // P = (I - K * H) * P
    float P00_temp = P00;
    float P01_temp = P01;
    
    P00 -= K0 * P00_temp;
    P01 -= K0 * P01_temp;
    P10 -= K1 * P00_temp;
    P11 -= K1 * P01_temp;
}

float KalmanFilter1D::getVelocity() const {
    return v;
}
