#pragma once

#include <Arduino.h>
#include <Protothread.h>
#include <Wire.h>
#include <ZumoMotors.h>

class Motor: public Protothread {
public:
    Motor(): mLeftSetPoint(0), mLeftValue(0), mRightSetPoint(0), mRightValue(0) { }

    void set(int leftSetPoint, int rightSetPoint);
    int getLeft() { return mLeftValue; }
    int getRight() { return mRightValue; }

    bool resume();

private:
    ZumoMotors mMotor;
    int mLeftSetPoint;
    int mLeftValue;
    int mRightSetPoint;
    int mRightValue;
};
