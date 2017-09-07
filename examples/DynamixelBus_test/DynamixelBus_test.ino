
// ========================================
// DynamixelBus Arduino library example
// ========================================

#include "DynamixelXL320.h"
//#include "DynamixelAX12.h"

// Name your robot!
DynamixelXL320 robot;
//DynamixelAX12 robot;

// Set some variables for incrementing position & LED colour
char rgb[] = "orgbypcw";
int ledColour = 0;
int servoPosition = 0;

// Set the default servoID to talk to
int servoID = 1;

int pin_led = 2;  // Internal LED
//int pin_led = LED_BUILTIN;  // Onboard LED
int pin_direction = 12;

void setup() {
  // Led
  pinMode(pin_led, OUTPUT);
  digitalWrite(pin_led, LOW);    // turn the LED on

  Serial.begin(1000000);
  Serial1.begin(115200);
  // Swap serial
  Serial.swap();

  // Initialise your robot
  robot.begin(Serial, pin_direction); // Hand in the serial object you're using

  Serial1.println("DynamixelBus_test started.");
}

void loop() {

  // LED colour test.. cycle between RGB, increment the colour and return from 0 to 3
  ledColour = (ledColour + 1) % 4;

  // Servo test
  for(int i=0; i<6; i++) {
    Serial1.print("Test Servo ID: ");Serial1.println(i);
    
    // test LED
    robot.setLed(i, &rgb[ledColour]);
    delay(1);
    // getJointPosition
    int currentPosition = robot.getJointPosition(i);
    Serial1.print("  Current position: "); Serial1.println(currentPosition);
    delay(1);
  }
  Serial1.println();
  
  delay(2000);
}
