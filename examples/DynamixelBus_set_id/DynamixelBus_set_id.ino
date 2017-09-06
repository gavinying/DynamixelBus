
// ========================================
// DynamixelBus Arduino library example
// ========================================

#include "DynamixelBus.h"

// Name your robot!
DynamixelBus robot;

// Set the target servoID to talk to
int currentId = -1;
int targetId = 1;

int pin_led = 2;  // Internal LED
//int pin_led = LED_BUILTIN;  // Onboard LED
int pin_direction = 12;

void setup() {
  // Led
  pinMode(pin_led, OUTPUT);
  digitalWrite(pin_led, LOW);    // turn the LED on

  // Talking standard serial, so connect servo data line to Digital TX 1
  // Comment out this line to talk software serial
  Serial.begin(1000000);
  Serial1.begin(115200);

  // Swap serial
  Serial.swap();

  // Initialise your robot
  robot.begin(Serial, pin_direction); // Hand in the serial object you're using

  delay(1000);
  
}

void loop() {
  // Check ID
  currentId = robot.searchId();
  if(currentId < 0) {
    Serial1.println("No device found.");
  }
  else {
    Serial1.print("Current Servo ID: "); Serial1.println(currentId);
    if(currentId != targetId) {
      robot.setId(targetId);
      Serial1.print("Set Servo ID to "); Serial1.println(targetId);
    }
  }
  delay(3000);
}
