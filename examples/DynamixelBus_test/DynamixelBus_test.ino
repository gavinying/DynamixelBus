// ========================================
// DynamixelBus Arduino library example
// Author: Ying Shaodong <helloysd@gmail.com>
// ========================================

#include "DynamixelXL320.h"
//#include "DynamixelAX12.h"

#define NUMBER_OF_SERVO   2
#define SERVO_ID(id)      (id+1)

DynamixelXL320 robot;
//DynamixelAX12 robot;

// Set some variables for incrementing position & LED colour
int ledColour = 0;  // color order: orgbypcw
int servoPositionMid = 512;   // middle position
int servoPositionMin = 51;
int servoPositionMax = 972;

int pin_direction = 12;

void setup() {

  Serial1.begin(115200);
  Serial1.println("DynamixelBus_test started.");

  Serial.begin(1000000);
  Serial.swap();  // Swap serial interface to serial2

  // Initialize robot controller
  robot.begin(Serial, pin_direction); // Hand in the serial object you're using
  // Initialize servo
  for(int i=0; i<NUMBER_OF_SERVO; i++) {
    robot.setControlMode(SERVO_ID(i), 2);
    robot.setGoalPosition(SERVO_ID(i), servoPositionMid);
  }

  Serial1.println("Robot initialized");
  delay(1000);
}

void loop() {

  // LED colour test.. cycle between RGB, increment the colour and return from 0 to 7
  ledColour = (ledColour + 1) % 8;

  // Servo test for all servos
  for(int i=0; i<NUMBER_OF_SERVO; i++) {
    Serial1.print("Test Servo ID: ");Serial1.println(SERVO_ID(i));

    // test LED
    robot.setLed(SERVO_ID(i), ledColour);

    // getPresentPosition
    int currentPosition = robot.getPresentPosition(SERVO_ID(i));
    Serial1.print("  Current position: "); Serial1.println(currentPosition);

    // setGoalPosition
    int goalPosition = currentPosition + 200;
    goalPosition = goalPosition>servoPositionMax ? servoPositionMin : goalPosition;
    robot.setGoalPosition(SERVO_ID(i), goalPosition);
    Serial1.print("  Set goal position to "); Serial1.println(goalPosition);

    Serial1.println();
  }

  delay(2000);
}
