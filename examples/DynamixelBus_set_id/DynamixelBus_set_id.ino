
// ========================================
// DynamixelBus Arduino library example
// ========================================

#include "DynamixelXL320.h"
//#include "DynamixelAX12.h"

// Name your robot!
DynamixelXL320 robot;
//DynamixelAX12 robot;

// Current servo ID
int currentId = 2;
// The servo ID to change to
// Hint: servo ID range: 1-253, don't set to 0, otherwise cannot change back to none-zero ID
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

  delay(3000);
  
}

void loop() {
  // Check Servo ID, make sure only one device connected. 
  int id = robot.searchId();
  if(id < 0) {
    Serial1.println("No device found.");
  }
  else if(id != currentId) {
    Serial1.print("Found servo ID: ");Serial1.print(id);Serial1.print(" , but not expected: ");Serial1.println(currentId);
  }
  else {
    if(currentId != targetId) {
      robot.setId(currentId, targetId);
      Serial1.print("Set current servo ID from "); Serial1.print(currentId); Serial1.print(" to "); Serial1.println(targetId);
    }
  }
  delay(2000);
}
