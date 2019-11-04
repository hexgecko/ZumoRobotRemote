#include <Arduino.h>
#include <Wire.h>
#include <ZumoShield.h>
#include <Protothread.h>

#include "Compass.h"

void Compass::setup() {
    // Initiate LSM303
    mCompass.init();

    // Enables accelerometer and magnetometer
    mCompass.enableDefault();

    mCompass.writeReg(LSM303::CRB_REG_M, CRB_REG_M_2_5GAUSS); // +/- 2.5 gauss sensitivity to hopefully avoid overflow problems
    mCompass.writeReg(LSM303::CRA_REG_M, CRA_REG_M_220HZ);

    // Set calibrated values to compass.m_max and compass.m_min
    mCompass.m_max.x = -64;
    mCompass.m_min.x = -221;
    mCompass.m_max.y = 70;
    mCompass.m_min.y = -118;
}

bool Compass::resume() {
    PT_BEGIN();

    for(;;) {
        mAvg = {0,0,0};
        for(mI = 0; mI<10; mI++) {
            mCompass.read();
            mAvg.x += mCompass.m.x;
            mAvg.y += mCompass.m.y;
            PT_YIELD();
        }
        mAvg.x /= 10.0;
        mAvg.y /= 10.0;

        // Avarage is the average measure of the magnetic vector.
        mHeading = heading(mAvg);
        PT_SLEEP(10);
    }

    PT_END();
}

template <typename T> float Compass::heading(LSM303::vector<T> v) {
    float x_scaled =  2.0*(float)(v.x - mCompass.m_min.x) / (mCompass.m_max.x - mCompass.m_min.x) - 1.0;
    float y_scaled =  2.0*(float)(v.y - mCompass.m_min.y) / (mCompass.m_max.y - mCompass.m_min.y) - 1.0;

    float angle = atan2(y_scaled, x_scaled)*180 / M_PI;
    if (angle < 0) angle += 360;
    
    return angle;
}
