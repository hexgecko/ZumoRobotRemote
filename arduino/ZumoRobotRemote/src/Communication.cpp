#include <Arduino.h>
#include <Protothread.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>

#include "Communication.h"

void Communication::setup() {
    Serial.begin(9600);
    mBluetooth.begin(9600);
}

bool Communication::resume() {
    PT_BEGIN();

    for(;;) {
        if(mBluetooth.available()) {
            char c = mBluetooth.read();
            Serial.write(c);

            if(c == '}') {
                mMsgBuffer[mMsgIndex] = '}';
                mMsgBuffer[mMsgIndex+1] = '\0';
                deserializeJson(mJsonMsg, mMsgBuffer);
                mMsgIndex = 0;

                const char* cmd = mJsonMsg[F("cmd")];

                if(strcmp(cmd, "motor") == 0) {
                    int left = mJsonMsg[F("left")];
                    int right = mJsonMsg[F("right")];
                    mMotor.set(left, right);

                } else if(strcmp(cmd, "status") == 0) {
                    mBluetooth.print("{\"left\":");
                    mBluetooth.print(mMotor.getLeft(), DEC);
                    mBluetooth.print(",\"right\":");
                    mBluetooth.print(mMotor.getRight(), DEC);
                    mBluetooth.print(",\"heading\":");
                    mBluetooth.print(mCompass.getHeading(), DEC);
                    mBluetooth.println("}");
                    mBluetooth.flush();
                }

            } else {
                mMsgBuffer[mMsgIndex++] = c;
            }
        }
        PT_SLEEP(1);
    }

    PT_END();
}