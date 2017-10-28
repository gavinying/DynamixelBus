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
int ledColour = 0;  // color order: orgybpcw
int servoPositionMid = 512;   // middle position
int servoPositionMin = 51;
int servoPositionMax = 972;

int servoPosition[NUMBER_OF_SERVO];

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
    servoPosition[i] = servoPositionMid;
    robot.setControlMode(SERVO_ID(i), 2);
    robot.setGoalPosition(SERVO_ID(i), servoPosition[i]);
  }

  Serial1.println("Robot initialized");
  delay(1000);
}

void loop() {

  // Prepare goal position
  for(int i=0;i<NUMBER_OF_SERVO; i++) {
    servoPosition[i] += 200;
    servoPosition[i] = servoPosition[i]>servoPositionMax ? servoPositionMin : servoPosition[i];
  }

  // setSyncGoalPosition
  robot.setSyncGoalPosition(NUMBER_OF_SERVO, servoPosition);

  delay(2000);
}
