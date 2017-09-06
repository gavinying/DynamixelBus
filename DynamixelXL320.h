/*
 Code based on:
 Dynamixel.cpp - Ax-12+ Half Duplex USART Comunication
 Copyright (c) 2011 Savage Electronics.
 And Dynamixel Pro library for OpenCM-9.04 made by ROBOTIS, LTD.
 And Arduino library for Dynamixel XL-320 made by hackerspace-adelaide/XL320

 Thanks to all the above authors for the great work!

 Modified to work with both packet sending and receiving functions.

 Modifications made by Ying Shaodong <helloysd@gmail.com>

 This file can be used and be modified by anyone under GPL v2.0

 */

#ifndef DYNAMIXEL_XL320_H_
#define DYNAMIXEL_XL320_H_

/*EEPROM Area*/
#define XL320_MODEL_NUMBER_L           0
#define XL320_MODEL_NUMBER_H           1
#define XL320_VERSION                  2
#define XL320_ID                       3
#define XL320_BAUD_RATE                4
#define XL320_RETURN_DELAY_TIME        5
#define XL320_CW_ANGLE_LIMIT_L         6
#define XL320_CW_ANGLE_LIMIT_H         7
#define XL320_CCW_ANGLE_LIMIT_L        8
#define XL320_CCW_ANGLE_LIMIT_H        9
#define XL320_CONTROL_MODE             11
#define XL320_LIMIT_TEMPERATURE        12
#define XL320_DOWN_LIMIT_VOLTAGE       13
#define XL320_UP_LIMIT_VOLTAGE         14
#define XL320_MAX_TORQUE_L             15
#define XL320_MAX_TORQUE_H             16
#define XL320_RETURN_LEVEL             17
#define XL320_ALARM_SHUTDOWN           18

/*RAM Area*/
#define XL320_TORQUE_ENABLE            24
#define XL320_LED                      25
#define XL320_D_GAIN                   27
#define XL320_I_GAIN                   28
#define XL320_P_GAIN                   29
#define XL320_GOAL_POSITION_L          30
#define XL320_GOAL_SPEED_L             32
#define XL320_GOAL_TORQUE              35
#define XL320_PRESENT_POSITION         37
#define XL320_PRESENT_SPEED            39
#define XL320_PRESENT_LOAD             41
#define XL320_PRESENT_VOLTAGE          45
#define XL320_PRESENT_TEMPERATURE      46
#define XL320_REGISTERED_INSTRUCTION   47
#define XL320_MOVING                   49
#define XL320_HARDWARE_ERROR           50
#define XL320_PUNCH                    51

#endif
