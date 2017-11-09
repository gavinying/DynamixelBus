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

#include "Arduino.h"
#include <DynamixelAX12.h>

//#define DEBUG

// Select the Switch to TX/RX Mode Pin
#define RX  0
#define TX  1
#define setDirection(Mode) digitalWrite(pin_direction,Mode);delay(5);

DynamixelAX12::DynamixelAX12() {
}

DynamixelAX12::~DynamixelAX12() {
}

void DynamixelAX12::begin(Stream &stream, int pin_d) {
  pin_direction = pin_d;
  pinMode(pin_direction, OUTPUT);
  this->stream = &stream;
  stream.setTimeout(10);  // timeout = 10 millisecond
}

int DynamixelAX12::searchId() {
  int ret = NO_ERROR;
  setDirection(TX);
  sendReadPacket(BROADCAST_ADDRESS, AX12_ID, 1);
  setDirection(RX);
  unsigned char buffer[MAXNUM_RX_PACKET];
  ret = receivePacket(buffer,MAXNUM_RX_PACKET);
  if(ret >= 0) {
    Dxl_Packet p(buffer,MAXNUM_RX_PACKET);
    if(p.isValid() && p.getParameterCount()>=1) {
      ret = p.getParameter(0);
    } else {
      ret = ERROR_INVALID;
    }
  }
  return ret;
}

int DynamixelAX12::setId(int id, int newId) {
  int ret = NO_ERROR;
  setDirection(TX);
  sendWriteU8Packet(id, AX12_ID, newId);
  setDirection(RX);
  unsigned char buffer[MAXNUM_RX_PACKET];
  ret = receivePacket(buffer,MAXNUM_RX_PACKET);
  if(ret >= 0) {
    Dxl_Packet p(buffer,MAXNUM_RX_PACKET);
    if(!p.isValid()) {
      ret = ERROR_INVALID;
    }
  }
  return ret;
}

int DynamixelAX12::getBaudrate(int id) {
  int ret = NO_ERROR;
  setDirection(TX);
  sendReadPacket(id, AX12_BAUD_RATE, 1);
  setDirection(RX);
  unsigned char buffer[MAXNUM_RX_PACKET];
  ret = receivePacket(buffer,MAXNUM_RX_PACKET);
  if(ret >= 0) {
    Dxl_Packet p(buffer,MAXNUM_RX_PACKET);
    if(p.isValid() && p.getParameterCount()>=1) {
      ret = p.getParameter(0);
    } else {
      #ifdef DEBUG
        Serial1.println("Response Invalid");
      #endif
      ret = ERROR_INVALID;
    }
  }
  return ret;
}

int DynamixelAX12::setBaudrate(int id, int value) {
  int ret = NO_ERROR;
  setDirection(TX);
  sendWriteU8Packet(id, AX12_BAUD_RATE, value);
  setDirection(RX);
  unsigned char buffer[MAXNUM_RX_PACKET];
  ret = receivePacket(buffer,MAXNUM_RX_PACKET);
  if(ret >= 0) {
    Dxl_Packet p(buffer,MAXNUM_RX_PACKET);
    if(!p.isValid()) {
      #ifdef DEBUG
        Serial1.println("Response Invalid");
      #endif
      ret = ERROR_INVALID;
    }
  }
  return ret;
}

int DynamixelAX12::setLed(int id, int value){
  int ret = NO_ERROR;
  value = value % 2;
  setDirection(TX);
  sendWriteU8Packet(id, AX12_LED, value);
  setDirection(RX);
  unsigned char buffer[MAXNUM_RX_PACKET];
  ret = receivePacket(buffer,MAXNUM_RX_PACKET);
  if(ret >= 0) {
    Dxl_Packet p(buffer,MAXNUM_RX_PACKET);
    if(!p.isValid()) {
      #ifdef DEBUG
        Serial1.println("Response Invalid");
      #endif
      ret = ERROR_INVALID;
    }
  }
  return ret;
}

int DynamixelAX12::getModelNumber(int id) {
  int ret = NO_ERROR;
  setDirection(TX);
  sendReadPacket(id, AX12_MODEL_NUMBER_L, 2);
  setDirection(RX);
  unsigned char buffer[MAXNUM_RX_PACKET];
  ret = receivePacket(buffer,MAXNUM_RX_PACKET);
  if(ret >= 0) {
    Dxl_Packet p(buffer,MAXNUM_RX_PACKET);
    if(p.isValid() && p.getParameterCount()>=2) {
      ret = p.getParameter(0) + (p.getParameter(1)<<8);
    } else {
      #ifdef DEBUG
        Serial1.println("Response Invalid");
      #endif
      ret = ERROR_INVALID;
    }
  }
  return ret;
}

int DynamixelAX12::setTorqueEnable(int id, bool value) {
  int ret = NO_ERROR;
  setDirection(TX);
  sendWriteU8Packet(id, AX12_TORQUE_ENABLE, value);
  setDirection(RX);
  unsigned char buffer[MAXNUM_RX_PACKET];
  ret = receivePacket(buffer,MAXNUM_RX_PACKET);
  if(ret >= 0) {
    Dxl_Packet p(buffer,MAXNUM_RX_PACKET);
    if(!p.isValid()) {
      #ifdef DEBUG
        Serial1.println("Response Invalid");
      #endif
      ret = ERROR_INVALID;
    }
  }
  return ret;
}

int DynamixelAX12::setControlMode(int id, int value) {
  int ret = NO_ERROR;
  // set control mode
  if(value == MODE_WHEEL) {
    // set ccw
    setDirection(TX);
    sendWriteU16Packet(id, AX12_CCW_ANGLE_LIMIT_L, 0);
    setDirection(RX);
    unsigned char buffer[MAXNUM_RX_PACKET];
    ret = receivePacket(buffer,MAXNUM_RX_PACKET);
    if(ret >= 0) {
      Dxl_Packet p(buffer,MAXNUM_RX_PACKET);
      if(!p.isValid()) {
        #ifdef DEBUG
          Serial1.println("Response Invalid");
        #endif
        ret = ERROR_INVALID;
      }
    }
    if(ret != NO_ERROR)
      return ret;
  } else if(value == MODE_JOINT) {
    // set ccw
    setDirection(TX);
    sendWriteU16Packet(id, AX12_CCW_ANGLE_LIMIT_L, 1023);
    setDirection(RX);
    unsigned char buffer[MAXNUM_RX_PACKET];
    ret = receivePacket(buffer,MAXNUM_RX_PACKET);
    if(ret >= 0) {
      Dxl_Packet p(buffer,MAXNUM_RX_PACKET);
      if(!p.isValid()) {
        #ifdef DEBUG
          Serial1.println("Response Invalid");
        #endif
        ret = ERROR_INVALID;
      }
    }
    if(ret != NO_ERROR)
      return ret;
  }
  else {
    return ERROR_INVALID;
  }
  return ret;
}

int DynamixelAX12::getGoalPosition(int id) {
  int ret = NO_ERROR;
  setDirection(TX);
  sendReadPacket(id, AX12_GOAL_POSITION, 2);
  setDirection(RX);
  unsigned char buffer[MAXNUM_RX_PACKET];
  ret = receivePacket(buffer,MAXNUM_RX_PACKET);
  if(ret >= 0) {
    Dxl_Packet p(buffer,MAXNUM_RX_PACKET);
    if(p.isValid() && p.getParameterCount()>=2) {
      ret = p.getParameter(0)|(p.getParameter(1)<<8);
    } else {
      #ifdef DEBUG
        Serial1.println("Response Invalid");
      #endif
      ret = ERROR_INVALID;
    }
  }
  return ret;
}

int DynamixelAX12::setGoalPosition(int id, int value) {
  int ret = NO_ERROR;
  setDirection(TX);
  sendWriteU16Packet(id, AX12_GOAL_POSITION, value);
  setDirection(RX);
  unsigned char buffer[MAXNUM_RX_PACKET];
  ret = receivePacket(buffer,MAXNUM_RX_PACKET);
  if(ret >= 0) {
    Dxl_Packet p(buffer,MAXNUM_RX_PACKET);
    if(!p.isValid()) {
      #ifdef DEBUG
        Serial1.println("Response Invalid");
      #endif
      ret = ERROR_INVALID;
    }
  }
  return ret;
}

int DynamixelAX12::getPresentPosition(int id) {
  int ret = NO_ERROR;
  setDirection(TX);
  sendReadPacket(id, AX12_PRESENT_POSITION, 2);
  setDirection(RX);
  unsigned char buffer[MAXNUM_RX_PACKET];
  ret = receivePacket(buffer,MAXNUM_RX_PACKET);
  if(ret >= 0) {
    Dxl_Packet p(buffer,MAXNUM_RX_PACKET);
    if(p.isValid() && p.getParameterCount()>=2) {
      ret = p.getParameter(0)|(p.getParameter(1)<<8);
    } else {
      #ifdef DEBUG
        Serial1.println("Response Invalid");
      #endif
      ret = ERROR_INVALID;
    }
  }
  return ret;
}


// Basic packet functions
void DynamixelAX12::sendReadPacket(int id, int address, int length){
  const int bufsize = 9;  // technically should be 8
  byte txbuffer[bufsize];
  Dxl_Packet p(txbuffer,bufsize,id,0x02,2,
    DXL_LOBYTE(address),
    DXL_LOBYTE(length));
  stream->write(txbuffer,bufsize);
  stream->flush();
  #ifdef DEBUG
    Serial1.print(" [DEBUG] Sent Read -> ");
    p.toStream(Serial1);
  #endif
}

void DynamixelAX12::sendWriteU8Packet(int id, int address, int value){
  const int bufsize = 9;   // technically should be 8
  byte txbuffer[bufsize];
  Dxl_Packet p(txbuffer,bufsize,id,0x03,2,
    DXL_LOBYTE(address),
    DXL_LOBYTE(value));
  stream->write(txbuffer,bufsize);
  stream->flush();
  #ifdef DEBUG
    Serial1.print(" [DEBUG] Sent WriteU8 -> ");
    p.toStream(Serial1);
  #endif
}

void DynamixelAX12::sendWriteU16Packet(int id, int address, int value){
  const int bufsize = 10;  // technically should be 9
  byte txbuffer[bufsize];
  Dxl_Packet p(txbuffer,bufsize,id,0x03,3,
    DXL_LOBYTE(address),
    DXL_LOBYTE(value),
    DXL_HIBYTE(value));
  stream->write(txbuffer,bufsize);
  stream->flush();
  #ifdef DEBUG
    Serial1.print(" [DEBUG] Sent WriteU16 -> ");
    p.toStream(Serial1);
  #endif
}

// from http://stackoverflow.com/a/133363/195061
#define FSM
#ifdef DEBUG
  #define STATE(x)  s_##x : if(!stream->readBytes(&BUFFER[I++],1)) goto s_timeout ; if(I>=SIZE) goto s_overflow; Serial1.print(BUFFER[I-1]); Serial1.print(" "); sn_##x :   // for debug
#else
  #define STATE(x)  s_##x : if(!stream->readBytes(&BUFFER[I++],1)) goto s_timeout ; if(I>=SIZE) goto s_overflow; sn_##x :
#endif
#define THISBYTE        (BUFFER[I-1])
#define NEXTSTATE(x)    goto s_##x
#define NEXTSTATE_NR(x) goto sn_##x
#define TIMEOUT         s_timeout :
#define OVERFLOW        s_overflow :

int DynamixelAX12::receivePacket(unsigned char *BUFFER, size_t SIZE) {
  int C;
  int I = 0;
  int length = 0;

  #ifdef DEBUG
  Serial1.print(" [DEBUG] Received -> ");
  #endif

  // state names normally name the last parsed symbol
  FSM {
    STATE(start) {
      if(THISBYTE==0xFF) NEXTSTATE(header_ff_1);
      I=0; NEXTSTATE(start);
    }
    STATE(header_ff_1) {
      if(THISBYTE==0xFF) NEXTSTATE(header_ff_2);
      I=0; NEXTSTATE(start);
    }
    STATE(header_ff_2) {
      // id = THISBYTE;
    }
    STATE(id) {
      length = THISBYTE;
    }
    STATE(length) {
      // instr = THISBYTE;
    }
    STATE(instr) {
      // check length because action and reboot commands have no parameters
      if(I-length>=3) NEXTSTATE(checksum);
    }
    STATE(params) {
      // check length and maybe skip to checksum
      if(I-length>=3) NEXTSTATE(checksum);
      // or keep reading params
      NEXTSTATE(params);
    }
    STATE(checksum) {
      #ifdef DEBUG
      Serial1.println();
      #endif
      // done
      return I;
    }
    TIMEOUT {
      #ifdef DEBUG
      Serial1.println("Response Timeout");
      #endif
      return ERROR_TIMEOUT;
    }
    OVERFLOW {
      #ifdef DEBUG
      Serial1.println("Response Overflow");
      #endif
      return ERROR_OVERFLOW;
    }
  }
}

DynamixelAX12::Dxl_Packet::Dxl_Packet(
  unsigned char *data,
  size_t data_size,
  unsigned char id,
  unsigned char instruction,
  int parameter_data_size,
  ...) {
    // [ff][ff][id][len] { [instr][params(parameter_data_size)][checksum] }
    unsigned int length=2+parameter_data_size;
    if(!data) {
      // [ff][ff][id][len] { [data(length)] }
      this->data_size = 4+length;
      this->data = (unsigned char*)malloc(data_size);
      this->freeData = true;
    } else {
      this->data = data;
      this->data_size = data_size;
      this->freeData = false;
    }
    this->data[0]=0xFF;
    this->data[1]=0xFF;
    this->data[2]=id;
    this->data[3]=length;
    this->data[4]=instruction;
    va_list args;
    va_start(args, parameter_data_size);
    for(int i=0;i<parameter_data_size;i++) {
      unsigned char arg = va_arg(args, int);
      this->data[5+i]=arg;
    }
    unsigned char checksum = this->update_checksum(this->data, 2, 1+length);
    this->data[5+parameter_data_size] = checksum;
    va_end(args);
}

DynamixelAX12::Dxl_Packet::Dxl_Packet(unsigned char *data, size_t size) {
  this->data = data;
  this->data_size = size;
  this->freeData = false;
}

DynamixelAX12::Dxl_Packet::~Dxl_Packet() {
  if(this->freeData==true) {
    free(this->data);
  }
}

void DynamixelAX12::Dxl_Packet::toStream(Stream &stream) {
  for(int i=0; i<getSize(); i++) {
    stream.print(this->data[i], HEX);
    stream.print(" ");
  }
  stream.print(" (valid: ");
  stream.println(isValid()?"yes)":"no)");
}

unsigned char DynamixelAX12::Dxl_Packet::getId() {
  return data[2];
}

int DynamixelAX12::Dxl_Packet::getLength() {
  return data[3];
}

int DynamixelAX12::Dxl_Packet::getSize() {
  return getLength()+4;
}

int DynamixelAX12::Dxl_Packet::getParameterCount() {
  return getLength()-2;
}

unsigned char DynamixelAX12::Dxl_Packet::getInstruction() {
  return data[4];
}

unsigned char DynamixelAX12::Dxl_Packet::getParameter(int n) {
  return data[5+n];
}

bool DynamixelAX12::Dxl_Packet::isValid() {
  int length = getLength();
  unsigned short storedChecksum = data[length+3];
  return storedChecksum == update_checksum(data, 2, 1+length);
}

unsigned char DynamixelAX12::Dxl_Packet::update_checksum(unsigned char *data_blk_ptr, int data_blk_start, int data_blk_size) {
  int sum = 0;
  for(int i=0; i<data_blk_size; i++) {
    sum += *(data_blk_ptr+data_blk_start+i);
  }
  return ~(sum&0xff);
}
