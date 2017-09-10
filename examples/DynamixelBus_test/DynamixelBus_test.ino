
// ========================================
// DynamixelBus Arduino library example
// ========================================

#include "DynamixelXL320.h"
//#include "DynamixelAX12.h"

// Name your robot!
DynamixelXL320 robot;
//DynamixelAX12 robot;

// Set the number of servo motors
int servoNum = 2;

// Set the default servoID to talk to
//int servoID = 1;

// Set some variables for incrementing position & LED colour
int ledColour = 0;  // color order: orgbypcw
int servoPosition = 0;

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

  // Initialise your robot controller
  robot.begin(Serial, pin_direction); // Hand in the serial object you're using
  
  // Configurate all servos
  for(int i=0; i<servoNum; i++) {
    robot.setControlMode(i, 2);
    delay(1);
    robot.setGoalPosition(i, 0);
    delay(1);
  }
  
  Serial1.println("DynamixelBus_test started.");
  delay(1000);
}

void loop() {

  // LED colour test.. cycle between RGB, increment the colour and return from 0 to 7
  ledColour = (ledColour + 1) % 8;
  
  // Servo test for all servos
  for(int i=0; i<servoNum; i++) {
    Serial1.print("Test Servo ID: ");Serial1.println(i);
    
    // test LED
    robot.setLed(i, ledColour);
    delay(1);
    // getPresentPosition
    int currentPosition = robot.getPresentPosition(i);
    Serial1.print("  Current position: "); Serial1.println(currentPosition);
    delay(1);
    // setGoalPosition
    int goalPosition = currentPosition + 200;
    goalPosition = goalPosition>1023 ? 1 : goalPosition;
    robot.setGoalPosition(i, goalPosition);
    delay(1);
  }
  Serial1.println();

  delay(2000);
}
