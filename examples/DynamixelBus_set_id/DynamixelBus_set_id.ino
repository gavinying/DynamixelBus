// ========================================
// DynamixelBus_set_id
// Author: Ying Shaodong <helloysd@gmail.com>
// Usage: 
//  1. Set expected currentId and newId, compile and program; 
//  2. The number of onboard LED flashing indicates current servo ID; 
//  3. The servo LED (RED) will be on if new ID is set successfully; 
// ========================================

#include "DynamixelXL320.h"
//#include "DynamixelAX12.h"

// Name your robot!
DynamixelXL320 robot;
//DynamixelAX12 robot;

// Current servo ID
int currentId = 7;
// The new servo ID
int newId = 1;

int pin_led = LED_BUILTIN;  // Onboard LED
int pin_direction = 12;

// led functions
void led_toggle(int pin) {
  int state = digitalRead(pin);  // get the current state of led pin
  digitalWrite(pin, !state);     // set pin to the opposite state
}
void led_on(int pin) {
  digitalWrite(pin, LOW);    // turn the LED on
}
void led_off(int pin) {
  digitalWrite(pin, HIGH);    // turn the LED off 
}

void setup() {
  // Led
  pinMode(pin_led, OUTPUT);
  led_off(pin_led);

  // Talking standard serial, so connect servo data line to Digital TX 1
  // Comment out this line to talk software serial
  Serial.begin(1000000);
  Serial1.begin(115200);

  // Swap serial
  Serial.swap();

  // Initialise your robot
  robot.begin(Serial, pin_direction); // Hand in the serial object you're using
  robot.setLed(BROADCAST_ADDRESS, LED_OFF); // LED off
  
  delay(3000);
}

void loop() {
  led_off(pin_led);
  delay(2000);
  
  // Check Servo ID, make sure only one device connected. 
  int id = robot.searchId();
  if(id <= 0) {
    Serial1.println("No device found.");
  }
  else {
    for(int i=0;i<id;i++) {
      led_on(pin_led);
      delay(200);
      led_off(pin_led);
      delay(200);
    }
    if(id == newId) {
      Serial1.print("Found servo ID: ");Serial1.print(id);Serial1.println(" ... No need to change. ");
      robot.setLed(id, LED_RED); // Red LED on in order to be compatible with AX-12 series
    }
    else if(id == currentId) {
      robot.setId(currentId, newId);
      Serial1.print("Set servo ID from "); Serial1.print(currentId); Serial1.print(" to "); Serial1.println(newId);
    }
    else {
      Serial1.print("Found servo ID: ");Serial1.print(id);Serial1.println(" ... But not expected. ");
    }
  }
  delay(2000);
}
