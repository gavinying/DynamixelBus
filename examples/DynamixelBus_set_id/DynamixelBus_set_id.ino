
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
// The new servo ID
int newId = 1;

char rgb[] = "rgbypcwo";

//int pin_led = 2;  // Internal LED
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
  led_on(pin_led);
  int id = robot.searchId();
  if(id < 0) {
    led_off(pin_led);
    Serial1.println("No device found.");
  }
  else if(id != currentId) {
    led_off(pin_led);
    Serial1.print("Found servo ID: ");Serial1.print(id);Serial1.print(" , but not expected: ");Serial1.println(currentId);
  }
  else {
    if(currentId != newId) {
      led_off(pin_led);
      robot.setId(currentId, newId);
      Serial1.print("Set current servo ID from "); Serial1.print(currentId); Serial1.print(" to "); Serial1.println(newId);
    }
  }
  // LED test.. let's randomly set the colour (0-7)
  robot.setLed(newId, &rgb[random(0,7)]);
  delay(2000);
}
