#include <Arduino.h>
#include <SoftwareSerial.h>

// Press the button on the HC-05 module when applying power to enter setup-mode.

static const byte ledPin = 13;

SoftwareSerial bluetooth(4, 5); // RX, TX

void sendCommand(const char* command) {
	bluetooth.println(command);
	Serial.println(command);

	digitalWrite(ledPin, HIGH);
	delay(500);
	while(bluetooth.available()) {
		Serial.write(bluetooth.read());
	}
	digitalWrite(ledPin, LOW);
	delay(100);
}

void setup(){

	pinMode(ledPin, OUTPUT);

	Serial.begin(9600);
	bluetooth.begin(38400);

	Serial.println("Serial and bluetooth initalized...");
	
	sendCommand("AT");
	sendCommand("AT+VERSION");
	sendCommand("AT+ROLE");
	sendCommand("AT+PSWD=1234");
	sendCommand("AT+NAME=SumoRobot");
}

void loop() {
}
