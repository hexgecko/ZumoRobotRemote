#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <ZumoShield.h>
#include <Protothread.h>

#define CRB_REG_M_2_5GAUSS 0x60 // CRB_REG_M value for magnetometer +/-2.5 gauss full scale
#define CRA_REG_M_220HZ    0x1C // CRA_REG_M value for magnetometer 220 Hz update rate

class Compass: public Protothread {
public:
    void setup();
    bool resume();

    float getHeading() { return mHeading; }

private:
    template <typename T> float heading(LSM303::vector<T> v);

    LSM303 mCompass;
    float mHeading;
    LSM303::vector<int32_t> mAvg;
    byte mI;
};
