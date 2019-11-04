#pragma once

#include <Arduino.h>
#include <Protothread.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>

#include "Motor.h"
#include "Compass.h"

class Communication: public Protothread {
public:
    Communication(byte rxPort, byte txPort, Motor& motor, Compass& compass):
        mBluetooth(rxPort, txPort),
        mMsgIndex(0),
        mMotor(motor),
        mCompass(compass) { }
    
    void setup();
    bool resume();

private:
    SoftwareSerial mBluetooth;
    StaticJsonDocument<64> mJsonMsg;
    char mMsgBuffer[65];
    byte mMsgIndex;
    Motor& mMotor;
    Compass& mCompass;
};
