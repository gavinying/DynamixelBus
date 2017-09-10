/*
 Code based on:
 Dynamixel.cpp - Ax-12+ Half Duplex USART Comunication
 Copyright (c) 2011 Savage Electronics.
 And Dynamixel Pro library for OpenCM-9.04 made by ROBOTIS, LTD.
 And Arduino library for Dynamixel XL-320 made by hackerspace-adelaide/XL320
 Thanks to all the above authors for the great work!

 Modifications made by Ying Shaodong <helloysd@gmail.com>
 1) Modified to work with both packet sending and receiving functions;
 2) Support debug function on ESP8266 Serial1 port

 This file can be used and be modified by anyone under GPL v2.0

 */

#ifndef DynamixelAX12_H_
#define DynamixelAX12_H_

#include <inttypes.h>
#include <Stream.h>
#include "Dynamixel.h"

#ifdef __cplusplus
extern "C" {
#endif

/*EEPROM Area*/
#define AX12_MODEL_NUMBER_L           0
#define AX12_MODEL_NUMBER_H           1
#define AX12_VERSION                  2
#define AX12_ID                       3
#define AX12_BAUD_RATE                4
#define AX12_RETURN_DELAY_TIME        5
#define AX12_CW_ANGLE_LIMIT_L         6
#define AX12_CW_ANGLE_LIMIT_H         7
#define AX12_CCW_ANGLE_LIMIT_L        8
#define AX12_CCW_ANGLE_LIMIT_H        9
#define AX12_LIMIT_TEMPERATURE        11
#define AX12_DOWN_LIMIT_VOLTAGE       12
#define AX12_UP_LIMIT_VOLTAGE         13
#define AX12_MAX_TORQUE_L             14
#define AX12_MAX_TORQUE_H             15
#define AX12_RETURN_LEVEL             16
#define AX12_ALARM_LED                17
#define AX12_ALARM_SHUTDOWN           18

/*RAM Area*/
#define AX12_TORQUE_ENABLE            24
#define AX12_LED                      25
#define AX12_CW_MARGIN                26
#define AX12_CCW_MARGIN               27
#define AX12_CW_SLOPE                 28
#define AX12_CCW_SLOPE                29
#define AX12_GOAL_POSITION            30
#define AX12_MOVING_SPEED             32
#define AX12_TORQUE_LIMIT             34
#define AX12_PRESENT_POSITION         36
#define AX12_PRESENT_SPEED            38
#define AX12_PRESENT_LOAD             40
#define AX12_PRESENT_VOLTAGE          42
#define AX12_PRESENT_TEMPERATURE      43
#define AX12_REGISTERED_INSTRUCTION   44
#define AX12_MOVING                   46
#define AX12_LOCK_EEPROM              47
#define AX12_PUNCH                    48

#ifdef __cplusplus
}
#endif

class DynamixelAX12 {
private:
  unsigned char pin_direction;
  Stream *stream;

public:
  DynamixelAX12();
  virtual ~DynamixelAX12();

  void begin(Stream &stream, int pin_d);

  int searchId();
  int setId(int id, int newId);
  int getBaudrate(int id);
  int setBaudrate(int id, int value);
  int setLed(int id, int value);
  int getModelNumber(int id);
  int setTorqueEnable(int id, bool value);
  int setControlMode(int id, int value);
  int getGoalPosition(int id);
  int setGoalPosition(int id, int value);
  int getPresentPosition(int id);

  void sendPingPacket();
  void sendReadPacket(int id, int address, int length);
  void sendWriteU8Packet(int id, int address, int value);
  void sendWriteU16Packet(int id, int address, int value);

  int receivePacket(unsigned char *buffer, size_t size);

  class Dxl_Packet {
    bool freeData;
  public:
    unsigned char *data;
    size_t data_size;

    Dxl_Packet(unsigned char *data, size_t size);
    Dxl_Packet(unsigned char *data, size_t size, unsigned char id, unsigned char instruction, int parameter_data_size, ...);

    ~Dxl_Packet();
    unsigned char getId();
    int getLength();
    int getSize();
    int getParameterCount();
    unsigned char getInstruction();
    unsigned char getParameter(int n);
    bool isValid();

    void toStream(Stream &stream);
    unsigned char update_checksum(unsigned char *data_blk_ptr, int data_blk_start, int data_blk_size);
  };

};

#endif
