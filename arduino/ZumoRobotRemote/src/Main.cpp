#include <Arduino.h>
#include <Wire.h>

#include "Communication.h"
#include "Motor.h"

Motor motor;
Compass compass;
Communication communication(4, 5, motor, compass);

void setup() {
  Wire.begin();

  communication.setup();
  compass.setup();
}

void loop() {
  communication.resume();
  motor.resume();
  compass.resume();
}