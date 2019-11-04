#include <Arduino.h>
#include <Protothread.h>
#include <ZumoMotors.h>

#include "Motor.h"

void Motor::set(int left, int right) {
    mLeftSetPoint = left;
    mRightSetPoint = right;
}

bool Motor::resume() {
    PT_BEGIN();

    for(;;) {
        if(mLeftValue < mLeftSetPoint) {
            mLeftValue++;
            mMotor.setLeftSpeed(mLeftValue);
        }
        
        if(mLeftValue > mLeftSetPoint) {
            mLeftValue--;
            mMotor.setLeftSpeed(mLeftValue);
        }

        if(mRightValue < mRightSetPoint) {
            mRightValue++;
            mMotor.setRightSpeed(mRightValue);
        }
        
        if(mRightValue > mRightSetPoint) {
            mRightValue--;
            mMotor.setRightSpeed(mRightValue);
        }

        PT_SLEEP(1);
    }

    PT_END();
}