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

#ifndef DynamixelXL320_H_
#define DynamixelXL320_H_

#include <inttypes.h>
#include <Stream.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BROADCAST_ADDRESS   (254)
#define MAXNUM_TX_PACKET    (255)
#define MAXNUM_RX_PACKET    (255)

/*utility for value*/
#define DXL_MAKEWORD(a, b)      ((unsigned short)(((unsigned char)(((unsigned long)(a)) & 0xff)) | ((unsigned short)((unsigned char)(((unsigned long)(b)) & 0xff))) << 8))
#define DXL_MAKEDWORD(a, b)     ((unsigned int)(((unsigned short)(((unsigned long)(a)) & 0xffff)) | ((unsigned int)((unsigned short)(((unsigned long)(b)) & 0xffff))) << 16))
#define DXL_LOWORD(l)           ((unsigned short)(((unsigned long)(l)) & 0xffff))
#define DXL_HIWORD(l)           ((unsigned short)((((unsigned long)(l)) >> 16) & 0xffff))
#define DXL_LOBYTE(w)           ((unsigned char)(((unsigned long)(w)) & 0xff))
#define DXL_HIBYTE(w)           ((unsigned char)((((unsigned long)(w)) >> 8) & 0xff))

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
#define XL320_GOAL_POSITION            30
#define XL320_GOAL_SPEED               32
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

#ifdef __cplusplus
}
#endif

class DynamixelXL320 {
private:
  unsigned char pin_direction;
  Stream *stream;

public:
  DynamixelXL320();
  virtual ~DynamixelXL320();

  void begin(Stream &stream, int pin_d);

  int searchId();
  void setId(int id, int targetId);
  int getBaudrate(int id);
  void setBaudrate(int id, int value);
  void setLed(int id, char led_color[]);

  int getJointPosition(int id);

  void sendPingPacket();
  void sendReadPacket(int id, int address, int length);
  void sendWriteU8Packet(int id, int address, int value);
  void sendWriteU16Packet(int id, int address, int value);

  int receivePacket(unsigned char *buffer, size_t size);

  class Packet {
    bool freeData;
  public:
    unsigned char *data;
    size_t data_size;

    Packet(unsigned char *data, size_t size);
    Packet(unsigned char *data, size_t size, unsigned char id, unsigned char instruction, int parameter_data_size, ...);

    ~Packet();
    unsigned char getId();
    int getLength();
    int getSize();
    int getParameterCount();
    unsigned char getInstruction();
    unsigned char getParameter(int n);
    bool isValid();

    void toStream(Stream &stream);
    unsigned short update_crc(unsigned short crc_accum, unsigned char *data_blk_ptr, unsigned short data_blk_size);
  };

};

#endif
