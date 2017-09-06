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

#ifndef DynamixelBus_H_
#define DynamixelBus_H_

#include <inttypes.h>
#include <Stream.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BROADCAST_ADDRESS   (254)
#define MAXNUM_TX_PACKET    (255)
#define MAXNUM_RX_PACKET    (255)

///////////////// utility for value ///////////////////////////
#define DXL_MAKEWORD(a, b)      ((unsigned short)(((unsigned char)(((unsigned long)(a)) & 0xff)) | ((unsigned short)((unsigned char)(((unsigned long)(b)) & 0xff))) << 8))
#define DXL_MAKEDWORD(a, b)     ((unsigned int)(((unsigned short)(((unsigned long)(a)) & 0xffff)) | ((unsigned int)((unsigned short)(((unsigned long)(b)) & 0xffff))) << 16))
#define DXL_LOWORD(l)           ((unsigned short)(((unsigned long)(l)) & 0xffff))
#define DXL_HIWORD(l)           ((unsigned short)((((unsigned long)(l)) >> 16) & 0xffff))
#define DXL_LOBYTE(w)           ((unsigned char)(((unsigned long)(w)) & 0xff))
#define DXL_HIBYTE(w)           ((unsigned char)((((unsigned long)(w)) >> 8) & 0xff))

unsigned short update_crc(unsigned short crc_accum, unsigned char *data_blk_ptr, unsigned short data_blk_size);

#ifdef __cplusplus
}
#endif

class DynamixelBus {
private:
  unsigned char pin_direction;
  //volatile char gbpParamEx[130 + 10];
  Stream *stream;

public:
  DynamixelBus();
  virtual ~DynamixelBus();

  void begin(Stream &stream, int pin_d);

  int searchId();
  void setId(int id);
  int getBaudrate(int id);
  void setBaudrate(int id, int value);
  void setLed(int id, char led_color[]);

  int getJointPosition(int id);

  void p2_sendPingPacket();
  void p2_sendReadPacket(int id, int address, int length);
  void p2_sendWriteU8Packet(int id, int address, int value);
  void p2_sendWriteU16Packet(int id, int address, int value);

  int p2_receivePacket(unsigned char *buffer, size_t size);

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
  };

};

#endif
