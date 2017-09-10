
// ========================================
// DynamixelBus Arduino library example
// ========================================

#include "DynamixelXL320.h"
//#include "DynamixelAX12.h"

// Name your robot!
DynamixelXL320 robot;
//DynamixelAX12 robot;

int servoId = 2;
int currentBaudrate = 3;  // 0: 9600, 1:57600, 2:115200, 3:1Mbps
int newBaudrate = 2;      // 0: 9600, 1:57600, 2:115200, 3:1Mbps

int pin_direction = 12;

void setup() {
  // Led
  pinMode(pin_led, OUTPUT);
  digitalWrite(pin_led, LOW);    // turn the LED on

  if(currentBaudrate == 0) 
    Serial.begin(9600);
  else if(currentBaudrate == 1)
    Serial.begin(57600);
  else if(currentBaudrate == 2)
    Serial.begin(115200);
  else if(currentBaudrate == 3)
    Serial.begin(1000000);
  
  Serial1.begin(115200);

  // Swap serial
  Serial.swap();

  // Initialise your robot
  robot.begin(Serial, pin_direction); // Hand in the serial object you're using

  delay(1000);
  
}

void loop() { 
  robot.setBaudrate(servoId, newBaudrate);
  Serial1.print("Set Baudrate to "); Serial1.println(newBaudrate);
  delay(2000);
}
