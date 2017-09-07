
// ========================================
// DynamixelBus Arduino library example
// ========================================

#include "DynamixelXL320.h"
//#include "DynamixelAX12.h"

// Name your robot!
DynamixelXL320 robot;
//DynamixelAX12 robot;

int targetBaud = 3;   // 0: 9600, 1:57600, 2:115200, 3:1Mbps

int pin_led = 2;  // Internal LED
//int pin_led = LED_BUILTIN;  // Onboard LED
int pin_direction = 12;

void setup() {
  // Led
  pinMode(pin_led, OUTPUT);
  digitalWrite(pin_led, LOW);    // turn the LED on

  //Serial.begin(115200);
  Serial.begin(1000000);
  Serial1.begin(115200);

  // Swap serial
  Serial.swap();

  // Initialise your robot
  robot.begin(Serial, pin_direction); // Hand in the serial object you're using

  delay(1000);
  
}

void loop() { 
  robot.setBaudrate(254, targetBaud);
  Serial1.print("Set Baudrate to "); Serial1.println(targetBaud);
  delay(2000);
}
