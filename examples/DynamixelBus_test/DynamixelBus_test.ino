
// ========================================
// DynamixelBus Arduino library example
// ========================================

#include "DynamixelBus.h"

// Name your robot!
DynamixelBus robot;

// Set some variables for incrementing position & LED colour
char rgb[] = "rgbypcwo";
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

  // Talking standard serial, so connect servo data line to Digital TX 1
  // Comment out this line to talk software serial
  Serial.begin(1000000);
  Serial1.begin(115200);

  // Swap serial
  Serial.swap();

  // Initialise your robot
  robot.begin(Serial, pin_direction); // Hand in the serial object you're using

  // I like fast moving servos, so set the joint speed to max!
  //robot.setJointSpeed(servoID, 1023);

  Serial1.println("DynamixelBus_test started ...");
}

void loop() {

  // LED colour test.. cycle between RGB, increment the colour and return 1 after 3
//  robot.setLed(servoID, &rgb[ledColour]);
//  ledColour = (ledColour + 1) % 3;
//  delay(2000);
  
  // Servo test.. let's randomly set the position (0-1023)
//  robot.moveJoint(servoID, random(0, 1023));

  // Servo test.. select a random servoID and colour
//  robot.moveJoint(random(1,4), random(0, 1023));

  // Servo test.. increment the servo position by 100 each loop
//  robot.moveJoint(servoID, servoPosition);
//  servoPosition = (servoPosition + 100) % 1023;

  // Servo test
  int currentPosition = robot.getJointPosition(servoID);
  Serial1.print("current position: "); Serial1.println(currentPosition);
  if(currentPosition != -1) {
    robot.setLed(servoID, &rgb[ledColour]);
    ledColour = (ledColour + 1) % 3;
  }
  
  // Set a delay to account for the receive delay period
  delay(1000);
}
