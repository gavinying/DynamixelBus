/*
 Code based on:
 Dynamixel.cpp - Ax-12+ Half Duplex USART Comunication
 Copyright (c) 2011 Savage Electronics.
 And Dynamixel Pro library for OpenCM-9.04 made by ROBOTIS, LTD.
 And Arduino library for Dynamixel XL-320 made by hackerspace-adelaide/AX12

 Thanks to all the above authors for the great work!

 Modified to work with both packet sending and receiving functions.

 Modifications made by Ying Shaodong <helloysd@gmail.com>

 This file can be used and be modified by anyone under GPL v2.0

 */

#include "Arduino.h"
#include <DynamixelAX12.h>

// Select the Switch to TX/RX Mode Pin
#define RX  0
#define TX  1
#define setDirection(Mode) digitalWrite(pin_direction,Mode);
#define ERROR_TIMEOUT   (-1)
#define ERROR_OVERFLOW  (-2)
#define ERROR_INVALID   (-3)

DynamixelAX12::DynamixelAX12() {
}

DynamixelAX12::~DynamixelAX12() {
}

void DynamixelAX12::begin(Stream &stream, int pin_d) {
  pin_direction = pin_d;
  pinMode(pin_direction, OUTPUT);
  this->stream = &stream;
  stream.setTimeout(1000);  // timeout = 1 second
}

int DynamixelAX12::searchId() {
  setDirection(TX);
  sendReadPacket(BROADCAST_ADDRESS, AX12_ID, 1);
  setDirection(RX);
  unsigned char buffer[MAXNUM_RX_PACKET];
  int ret = receivePacket(buffer,MAXNUM_RX_PACKET);
  if(ret < 0) {
    return ret;
  }
  else {
    Packet p(buffer,MAXNUM_RX_PACKET);
    if(p.isValid() && p.getParameterCount()>=1) {
      return p.getParameter(0);
    } else {
      return ERROR_INVALID;
    }
  }
}

void DynamixelAX12::setId(int targetId) {
  setDirection(TX);
  sendWriteU8Packet(BROADCAST_ADDRESS, AX12_ID, targetId);
}

int DynamixelAX12::getBaudrate(int id) {
  setDirection(TX);
  sendReadPacket(id, AX12_BAUD_RATE, 1);
  setDirection(RX);
  unsigned char buffer[MAXNUM_RX_PACKET];
  int ret = this->receivePacket(buffer,MAXNUM_RX_PACKET);
  if(ret < 0) {
    return ret;
  }
  else {
    Packet p(buffer,MAXNUM_RX_PACKET);
    if(p.isValid() && p.getParameterCount()>=1) {
      return p.getParameter(0);
    } else {
      return ERROR_INVALID;
    }
  }
}

void DynamixelAX12::setBaudrate(int id, int value) {
  setDirection(TX);
  sendWriteU8Packet(id, AX12_BAUD_RATE, value);
}

void DynamixelAX12::setLed(int id, char led_color[]){
  int val = 0;
  if(led_color[0] == 'o'){
    val = 0;
  }
  else if(led_color[0] == 'r'){
    val = 1;
  }
  else if(led_color[0] == 'g'){
    val = 2;
  }
  else if(led_color[0] == 'y'){
    val = 3;
  }
  else if(led_color[0] == 'b'){
    val = 4;
  }
  else if(led_color[0] == 'p'){
    val = 5;
  }
  else if(led_color[0] == 'c'){
    val = 6;
  }
  else if(led_color[0] == 'w'){
    val = 7;
  }
  setDirection(TX);
  sendWriteU8Packet(id, AX12_LED, val);
}

int DynamixelAX12::getJointPosition(int id) {
  setDirection(TX);
  sendReadPacket(id, AX12_PRESENT_POSITION, 2);
  setDirection(RX);
  unsigned char buffer[MAXNUM_RX_PACKET];
  int ret = this->receivePacket(buffer,MAXNUM_RX_PACKET);
  if(ret < 0) {
    return ret;
  }
  else {
    Packet p(buffer,MAXNUM_RX_PACKET);
    if(p.isValid() && p.getParameterCount()>=2) {
      return (p.getParameter(0))|(p.getParameter(1)<<8);
    } else {
      return ERROR_INVALID;
    }
  }
}

void DynamixelAX12::sendReadPacket(int id, int address, int length){
    const int bufsize = 9;  // technically should be 8, but need to send one more
    byte txbuffer[bufsize];
    Packet p(txbuffer,bufsize,id,0x02,2,
      DXL_LOBYTE(address),
      DXL_LOBYTE(length));
    stream->write(txbuffer,bufsize);
    stream->flush();
}

void DynamixelAX12::sendWriteU8Packet(int id, int address, int value){
    const int bufsize = 8;
    byte txbuffer[bufsize];
    Packet p(txbuffer,bufsize,id,0x03,2,
      DXL_LOBYTE(address),
      DXL_LOBYTE(value));
    stream->write(txbuffer,bufsize);
    stream->flush();
}

void DynamixelAX12::sendWriteU16Packet(int id, int address, int value){
    const int bufsize = 9;
    byte txbuffer[bufsize];
    Packet p(txbuffer,bufsize,id,0x03,3,
      DXL_LOBYTE(address),
      DXL_LOBYTE(value),
      DXL_HIBYTE(value));
    stream->write(txbuffer,bufsize);
    stream->flush();
}

// from http://stackoverflow.com/a/133363/195061
#define FSM
//#define STATE(x)        s_##x : if(!stream->readBytes(&BUFFER[I++],1)) goto s_timeout ; if(I>=SIZE) goto s_overflow; Serial1.println(BUFFER[I-1]); sn_##x :   // for debug
#define STATE(x)        s_##x : if(!stream->readBytes(&BUFFER[I++],1)) goto s_timeout ; if(I>=SIZE) goto s_overflow; sn_##x :
#define THISBYTE        (BUFFER[I-1])
#define NEXTSTATE(x)    goto s_##x
#define NEXTSTATE_NR(x) goto sn_##x
#define TIMEOUT         s_timeout :
#define OVERFLOW        s_overflow :

int DynamixelAX12::receivePacket(unsigned char *BUFFER, size_t SIZE) {
    int C;
    int I = 0;
    int length = 0;

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
        //Serial1.print("length="); Serial1.println(length);
        // instr = THISBYTE;
      }
      STATE(instr) {
        //Serial1.print("STATE(instr) I=");Serial1.println(I);   // for debug
        // check length because action and reboot commands have no parameters
        if(I-length>=3) NEXTSTATE(checksum);
      }
      STATE(params) {
        //Serial1.print("STATE(params) I=");Serial1.println(I);   // for debug
        // check length and maybe skip to checksum
        if(I-length>=3) NEXTSTATE(checksum);
        // or keep reading params
        NEXTSTATE(params);
      }
      STATE(checksum) {
        //Serial1.print("STATE(checksum) I=");Serial1.println(I);   // for debug
        // done
        return I;
      }
      TIMEOUT {
        return ERROR_TIMEOUT;
      }
      OVERFLOW {
        return ERROR_OVERFLOW;
      }
    }
}

DynamixelAX12::Packet::Packet(
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

DynamixelAX12::Packet::Packet(unsigned char *data, size_t size) {
    this->data = data;
    this->data_size = size;
    this->freeData = false;
}

DynamixelAX12::Packet::~Packet() {
    if(this->freeData==true) {
      free(this->data);
    }
}

void DynamixelAX12::Packet::toStream(Stream &stream) {
  stream.print("id: ");
  stream.println(this->getId(),DEC);
  stream.print("length: ");
  stream.println(this->getLength(),DEC);
  stream.print("instruction: ");
  stream.println(this->getInstruction(),HEX);
  stream.print("parameter count: ");
  stream.println(this->getParameterCount(), DEC);
  for(int i=0;i<this->getParameterCount(); i++) {
    stream.print(this->getParameter(i),HEX);
    if(i<this->getParameterCount()-1) {
      stream.print(",");
    }
  }
  stream.println();
  stream.print("valid: ");
  stream.println(this->isValid()?"yes":"no");
}

unsigned char DynamixelAX12::Packet::getId() {
    return data[2];
}

int DynamixelAX12::Packet::getLength() {
    return data[3];
}

int DynamixelAX12::Packet::getSize() {
    return getLength()+4;
}

int DynamixelAX12::Packet::getParameterCount() {
    return getLength()-2;
}

unsigned char DynamixelAX12::Packet::getInstruction() {
    return data[4];
}

unsigned char DynamixelAX12::Packet::getParameter(int n) {
    return data[5+n];
}

bool DynamixelAX12::Packet::isValid() {
    int length = getLength();
    unsigned short storedChecksum = data[length+3];
    return storedChecksum == this->update_checksum(data, 2, 1+length);
}

unsigned char DynamixelAX12::Packet::update_checksum(unsigned char *data_blk_ptr, int data_blk_start, int data_blk_size) {
  int sum = 0;
  for(int i=0; i<data_blk_size; i++) {
    sum += *(data_blk_ptr+data_blk_start+i);
  }
  return ~(sum&0xff);
}
