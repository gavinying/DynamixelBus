// ========================================
// DynamixelBus Arduino library example
// Author: Ying Shaodong <helloysd@gmail.com>
// ========================================

#include "DynamixelXL320.h"
//#include "DynamixelAX12.h"

DynamixelXL320 robot;
//DynamixelAX12 robot;

// Set the servoID to talk to
int servoId = 2;

// Set some variables for incrementing position & LED colour
int ledColour = 0;  // color order: orgbypcw
int servoPositionMid = 512;   // middle position
int servoPositionMin = 51;
int servoPositionMax = 972;

int pin_led = LED_BUILTIN;  // Onboard LED
int pin_direction = 12;

void setup() {
  // Led
  pinMode(pin_led, OUTPUT);
  digitalWrite(pin_led, LOW);    // turn the LED on

  Serial1.begin(115200);
  Serial1.println("DynamixelBus_test started.");

  Serial.begin(1000000);
  Serial.swap();  // Swap serial interface to serial2

  // Initialize robot controller
  robot.begin(Serial, pin_direction); // Hand in the serial object you're using

  // Initialize servo
  robot.setControlMode(servoId, 2);
  robot.setGoalPosition(servoId, servoPositionMid);

  Serial1.println("Robot initialized");
  delay(1000);
}

void loop() {

  Serial1.print("Test Servo ID: ");Serial1.println(servoId);

  // LED colour test.. cycle between RGB, increment the colour and return from 0 to 7
  ledColour = (ledColour + 1) % 8;

  // test LED
  robot.setLed(servoId, ledColour);

  // getPresentPosition
  int currentPosition = robot.getPresentPosition(servoId);
  Serial1.print("  Current position: "); Serial1.println(currentPosition);

  // setGoalPosition
  int goalPosition = currentPosition + 200;
  goalPosition = goalPosition>servoPositionMax ? servoPositionMin : goalPosition;
  robot.setGoalPosition(servoId, goalPosition);
  Serial1.print("  Set goal position to "); Serial1.println(goalPosition);

  Serial1.println();
  delay(2000);
}
