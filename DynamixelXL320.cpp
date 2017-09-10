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
#include <DynamixelXL320.h>

//#define DEBUG

// Select the Switch to TX/RX Mode Pin
#define RX  0
#define TX  1
#define setDirection(Mode) digitalWrite(pin_direction,Mode);

DynamixelXL320::DynamixelXL320() {
}

DynamixelXL320::~DynamixelXL320() {
}

void DynamixelXL320::begin(Stream &stream, int pin_d) {
  pin_direction = pin_d;
  pinMode(pin_direction, OUTPUT);
  this->stream = &stream;
  stream.setTimeout(10);  // timeout = 10 millisecond
}

int DynamixelXL320::searchId() {
  int ret = NO_ERROR;
  setDirection(TX);
  sendPingPacket();
  setDirection(RX);
  unsigned char buffer[MAXNUM_RX_PACKET];
  ret = receivePacket(buffer,MAXNUM_RX_PACKET);
  if(ret >= 0) {
    DxlV2_Packet p(buffer,MAXNUM_RX_PACKET);
    if(!p.isValid()) {
      ret = ERROR_INVALID;
    }
    else {
      ret = p.getId();
    }
  }
  return ret;
}

int DynamixelXL320::setId(int id, int newId) {
  int ret = NO_ERROR;
  setDirection(TX);
  sendWriteU8Packet(id, XL320_ID, newId);
  setDirection(RX);
  unsigned char buffer[MAXNUM_RX_PACKET];
  ret = receivePacket(buffer,MAXNUM_RX_PACKET);
  if(ret >= 0) {
    DxlV2_Packet p(buffer,MAXNUM_RX_PACKET);
    if(!p.isValid()) {
      ret = ERROR_INVALID;
    }
  }
  return ret;
}

int DynamixelXL320::getBaudrate(int id) {
  int ret = NO_ERROR;
  setDirection(TX);
  sendReadPacket(id, XL320_BAUD_RATE, 1);
  setDirection(RX);
  unsigned char buffer[MAXNUM_RX_PACKET];
  ret = receivePacket(buffer,MAXNUM_RX_PACKET);
  if(ret >= 0) {
    DxlV2_Packet p(buffer,MAXNUM_RX_PACKET);
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

int DynamixelXL320::setBaudrate(int id, int value) {
  int ret = NO_ERROR;
  setDirection(TX);
  sendWriteU8Packet(id, XL320_BAUD_RATE, value);
  setDirection(RX);
  unsigned char buffer[MAXNUM_RX_PACKET];
  ret = receivePacket(buffer,MAXNUM_RX_PACKET);
  if(ret >= 0) {
    DxlV2_Packet p(buffer,MAXNUM_RX_PACKET);
    if(!p.isValid()) {
      #ifdef DEBUG
        Serial1.println("Response Invalid");
      #endif
      ret = ERROR_INVALID;
    }
  }
  return ret;
}

int DynamixelXL320::setLed(int id, int value){
  int ret = NO_ERROR;
  value = value % 8; 
  setDirection(TX);
  sendWriteU8Packet(id, XL320_LED, value);
  setDirection(RX);
  unsigned char buffer[MAXNUM_RX_PACKET];
  ret = receivePacket(buffer,MAXNUM_RX_PACKET);
  if(ret >= 0) {
    DxlV2_Packet p(buffer,MAXNUM_RX_PACKET);
    if(!p.isValid()) {
      #ifdef DEBUG
        Serial1.println("Response Invalid");
      #endif
      ret = ERROR_INVALID;
    }
  }
  return ret;
}

int DynamixelXL320::setTorqueEnable(int id, bool value) {
  int ret = NO_ERROR;
  setDirection(TX);
  sendWriteU8Packet(id, XL320_TORQUE_ENABLE, value);
  setDirection(RX);
  unsigned char buffer[MAXNUM_RX_PACKET];
  ret = receivePacket(buffer,MAXNUM_RX_PACKET);
  if(ret >= 0) {
    DxlV2_Packet p(buffer,MAXNUM_RX_PACKET);
    if(!p.isValid()) {
      #ifdef DEBUG
        Serial1.println("Response Invalid");
      #endif
      ret = ERROR_INVALID;
    }
  }
  return ret;
}

int DynamixelXL320::setControlMode(int id, int value) {
  int ret = NO_ERROR;
  // disable torque
  ret = setTorqueEnable(id, false);
  if(ret != NO_ERROR)
    return ret;
  // set control mode
  setDirection(TX);
  sendWriteU8Packet(id, XL320_CONTROL_MODE, value);
  setDirection(RX);
  unsigned char buffer[MAXNUM_RX_PACKET];
  ret = receivePacket(buffer,MAXNUM_RX_PACKET);
  if(ret >= 0) {
    DxlV2_Packet p(buffer,MAXNUM_RX_PACKET);
    if(!p.isValid()) {
      #ifdef DEBUG
        Serial1.println("Response Invalid");
      #endif
      ret = ERROR_INVALID;
    }
  }
  if(ret != NO_ERROR)
    return ret;
  // enable torque
  ret = setTorqueEnable(id, true);
  return ret;
}

int DynamixelXL320::getGoalPosition(int id) {
  int ret = NO_ERROR;
  setDirection(TX);
  sendReadPacket(id, XL320_GOAL_POSITION, 2);
  setDirection(RX);
  unsigned char buffer[MAXNUM_RX_PACKET];
  ret = receivePacket(buffer,MAXNUM_RX_PACKET);
  if(ret >= 0) {
    DxlV2_Packet p(buffer,MAXNUM_RX_PACKET);
    if(p.isValid() && p.getParameterCount()>=3) {
      ret = (p.getParameter(1))|(p.getParameter(2)<<8);
    } else {
      #ifdef DEBUG
        Serial1.println("Response Invalid");
      #endif
      ret = ERROR_INVALID;
    }
  }
  return ret;
}

int DynamixelXL320::setGoalPosition(int id, int value) {
  int ret = NO_ERROR;
  setDirection(TX);
  sendWriteU16Packet(id, XL320_GOAL_POSITION, value);
  setDirection(RX);
  unsigned char buffer[MAXNUM_RX_PACKET];
  ret = receivePacket(buffer,MAXNUM_RX_PACKET);
  if(ret >= 0) {
    DxlV2_Packet p(buffer,MAXNUM_RX_PACKET);
    if(!p.isValid()) {
      #ifdef DEBUG
        Serial1.println("Response Invalid");
      #endif
      ret = ERROR_INVALID;
    }
  }
  return ret;
}

int DynamixelXL320::getPresentPosition(int id) {
  int ret = NO_ERROR;
  setDirection(TX);
  sendReadPacket(id, XL320_PRESENT_POSITION, 2);
  setDirection(RX);
  unsigned char buffer[MAXNUM_RX_PACKET];
  ret = receivePacket(buffer,MAXNUM_RX_PACKET);
  if(ret >= 0) {
    DxlV2_Packet p(buffer,MAXNUM_RX_PACKET);
    if(p.isValid() && p.getParameterCount()>=3) {
      ret = (p.getParameter(1))|(p.getParameter(2)<<8);
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
void DynamixelXL320::sendPingPacket(){
  const int bufsize = 11;   // should be 10
  byte txbuffer[bufsize];
  DxlV2_Packet p(txbuffer,bufsize,BROADCAST_ADDRESS,1,0);
  stream->write(txbuffer,bufsize);
  stream->flush();
  #ifdef DEBUG
    Serial1.print(" [DEBUG] Sent Ping -> ");
    p.toStream(Serial1);
  #endif
}

void DynamixelXL320::sendReadPacket(int id, int address, int length){
  const int bufsize = 15;   // It should be 14 according to ROBOTIS e-Manual v1.27.00
  byte txbuffer[bufsize];
  DxlV2_Packet p(txbuffer,bufsize,id,0x02,4,
    DXL_LOBYTE(address),
    DXL_HIBYTE(address),
    DXL_LOBYTE(length),
    DXL_HIBYTE(length));
  stream->write(txbuffer,bufsize);
  stream->flush();
  #ifdef DEBUG
    Serial1.print(" [DEBUG] Sent Read -> ");
    p.toStream(Serial1);
  #endif
}

void DynamixelXL320::sendWriteU8Packet(int id, int address, int value){
  const int bufsize = 14;   // should be 13
  byte txbuffer[bufsize];
  DxlV2_Packet p(txbuffer,bufsize,id,0x03,3,
    DXL_LOBYTE(address),
    DXL_HIBYTE(address),
    DXL_LOBYTE(value));
  stream->write(txbuffer,bufsize);
  stream->flush();
  #ifdef DEBUG
    Serial1.print(" [DEBUG] Sent WriteU8 -> ");
    p.toStream(Serial1);
  #endif
}

void DynamixelXL320::sendWriteU16Packet(int id, int address, int value){
  const int bufsize = 16;   // should be 14
  byte txbuffer[bufsize];
  DxlV2_Packet p(txbuffer,bufsize,id,0x03,4,
    DXL_LOBYTE(address),
    DXL_HIBYTE(address),
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
  #define STATE(x)  s_##x : if(!stream->readBytes(&BUFFER[I++],1)) goto s_timeout ; if(I>=SIZE) goto s_overflow; Serial1.print(BUFFER[I-1],HEX); Serial1.print(" "); sn_##x :   // for debug
#else
  #define STATE(x)  s_##x : if(!stream->readBytes(&BUFFER[I++],1)) goto s_timeout ; if(I>=SIZE) goto s_overflow; sn_##x :
#endif
#define THISBYTE        (BUFFER[I-1])
#define NEXTSTATE(x)    goto s_##x
#define NEXTSTATE_NR(x) goto sn_##x
#define TIMEOUT         s_timeout :
#define OVERFLOW        s_overflow :

int DynamixelXL320::receivePacket(unsigned char *BUFFER, size_t SIZE) {
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
      if(THISBYTE==0xFD) NEXTSTATE(header_fd);
      // yet more 0xFF's? stay in this state
      if(THISBYTE==0xFF) NEXTSTATE(header_ff_2);
      // anything else? restart
      I=0; NEXTSTATE(start);
    }
    STATE(header_fd) {
      // reading reserved, could be anything in theory, normally 0
    }
    STATE(header_reserved) {
      // id = THISBYTE;
    }
    STATE(id) {
      length = THISBYTE;
    }
    STATE(length_1) {
      length += THISBYTE<<8;
    }
    STATE(length_2) {
      // instr = THISBYTE;
    }
    STATE(instr) {
      // check length because
      // action and reboot commands have no parameters
      if(I-length>=5) NEXTSTATE(checksum_1);
    }
    STATE(params) {
      // check length and maybe skip to checksum
      if(I-length>=5) NEXTSTATE(checksum_1);
      // or keep reading params
      NEXTSTATE(params);
    }
    STATE(checksum_1) {
    }
    STATE(checksum_2) {
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

DynamixelXL320::DxlV2_Packet::DxlV2_Packet(unsigned char *data, size_t data_size, unsigned char id, unsigned char instruction, int parameter_data_size, ...) {
  // [ff][ff][fd][00][id][len1][len2] { [instr][params(parameter_data_size)][crc1][crc2] }
  unsigned int length=3+parameter_data_size;
  if(!data) {
    // [ff][ff][fd][00][id][len1][len2] { [data(length)] }
    this->data_size = 7+length;
    this->data = (unsigned char*)malloc(data_size);
    this->freeData = true;
  } else {
    this->data = data;
    this->data_size = data_size;
    this->freeData = false;
  }
  this->data[0]=0xFF;
  this->data[1]=0xFF;
  this->data[2]=0xFD;
  this->data[3]=0x00;
  this->data[4]=id;
  this->data[5]=length&0xff;
  this->data[6]=(length>>8)&0xff;
  this->data[7]=instruction;
  va_list args;
  va_start(args, parameter_data_size);
  for(int i=0;i<parameter_data_size;i++) {
    unsigned char arg = va_arg(args, int);
    this->data[8+i]=arg;
  }
  unsigned short crc = this->update_crc(0,this->data,getSize()-2);
  this->data[8+parameter_data_size]=crc&0xff;
  this->data[9+parameter_data_size]=(crc>>8)&0xff;
  va_end(args);
}

DynamixelXL320::DxlV2_Packet::DxlV2_Packet(unsigned char *data, size_t size) {
  this->data = data;
  this->data_size = size;
  this->freeData = false;
}

DynamixelXL320::DxlV2_Packet::~DxlV2_Packet() {
  if(this->freeData==true) {
    free(this->data);
  }
}

void DynamixelXL320::DxlV2_Packet::toStream(Stream &stream) {
  for(int i=0; i<getSize(); i++) {
    stream.print(this->data[i], HEX);
    stream.print(" ");
  }
  stream.print(" (valid: ");
  stream.println(isValid()?"yes)":"no)");
}

unsigned char DynamixelXL320::DxlV2_Packet::getId() {
  return data[4];
}

int DynamixelXL320::DxlV2_Packet::getLength() {
  return data[5]+((data[6]&0xff)<<8);
}

int DynamixelXL320::DxlV2_Packet::getSize() {
  return getLength()+7;
}

int DynamixelXL320::DxlV2_Packet::getParameterCount() {
  return getLength()-3;
}

unsigned char DynamixelXL320::DxlV2_Packet::getInstruction() {
  return data[7];
}

unsigned char DynamixelXL320::DxlV2_Packet::getParameter(int n) {
  return data[8+n];
}

bool DynamixelXL320::DxlV2_Packet::isValid() {
  int length = getLength();
  unsigned short storedChecksum = data[length+5]+(data[length+6]<<8);
  return storedChecksum == update_crc(0,data,length+5);
}

unsigned short DynamixelXL320::DxlV2_Packet::update_crc(unsigned short crc_accum, unsigned char *data_blk_ptr, unsigned short data_blk_size) {
  unsigned short i, j;
  unsigned short crc_table[256] = {
    0x0000, 0x8005, 0x800F, 0x000A, 0x801B, 0x001E, 0x0014, 0x8011,
    0x8033, 0x0036, 0x003C, 0x8039, 0x0028, 0x802D, 0x8027, 0x0022,
    0x8063, 0x0066, 0x006C, 0x8069, 0x0078, 0x807D, 0x8077, 0x0072,
    0x0050, 0x8055, 0x805F, 0x005A, 0x804B, 0x004E, 0x0044, 0x8041,
    0x80C3, 0x00C6, 0x00CC, 0x80C9, 0x00D8, 0x80DD, 0x80D7, 0x00D2,
    0x00F0, 0x80F5, 0x80FF, 0x00FA, 0x80EB, 0x00EE, 0x00E4, 0x80E1,
    0x00A0, 0x80A5, 0x80AF, 0x00AA, 0x80BB, 0x00BE, 0x00B4, 0x80B1,
    0x8093, 0x0096, 0x009C, 0x8099, 0x0088, 0x808D, 0x8087, 0x0082,
    0x8183, 0x0186, 0x018C, 0x8189, 0x0198, 0x819D, 0x8197, 0x0192,
    0x01B0, 0x81B5, 0x81BF, 0x01BA, 0x81AB, 0x01AE, 0x01A4, 0x81A1,
    0x01E0, 0x81E5, 0x81EF, 0x01EA, 0x81FB, 0x01FE, 0x01F4, 0x81F1,
    0x81D3, 0x01D6, 0x01DC, 0x81D9, 0x01C8, 0x81CD, 0x81C7, 0x01C2,
    0x0140, 0x8145, 0x814F, 0x014A, 0x815B, 0x015E, 0x0154, 0x8151,
    0x8173, 0x0176, 0x017C, 0x8179, 0x0168, 0x816D, 0x8167, 0x0162,
    0x8123, 0x0126, 0x012C, 0x8129, 0x0138, 0x813D, 0x8137, 0x0132,
    0x0110, 0x8115, 0x811F, 0x011A, 0x810B, 0x010E, 0x0104, 0x8101,
    0x8303, 0x0306, 0x030C, 0x8309, 0x0318, 0x831D, 0x8317, 0x0312,
    0x0330, 0x8335, 0x833F, 0x033A, 0x832B, 0x032E, 0x0324, 0x8321,
    0x0360, 0x8365, 0x836F, 0x036A, 0x837B, 0x037E, 0x0374, 0x8371,
    0x8353, 0x0356, 0x035C, 0x8359, 0x0348, 0x834D, 0x8347, 0x0342,
    0x03C0, 0x83C5, 0x83CF, 0x03CA, 0x83DB, 0x03DE, 0x03D4, 0x83D1,
    0x83F3, 0x03F6, 0x03FC, 0x83F9, 0x03E8, 0x83ED, 0x83E7, 0x03E2,
    0x83A3, 0x03A6, 0x03AC, 0x83A9, 0x03B8, 0x83BD, 0x83B7, 0x03B2,
    0x0390, 0x8395, 0x839F, 0x039A, 0x838B, 0x038E, 0x0384, 0x8381,
    0x0280, 0x8285, 0x828F, 0x028A, 0x829B, 0x029E, 0x0294, 0x8291,
    0x82B3, 0x02B6, 0x02BC, 0x82B9, 0x02A8, 0x82AD, 0x82A7, 0x02A2,
    0x82E3, 0x02E6, 0x02EC, 0x82E9, 0x02F8, 0x82FD, 0x82F7, 0x02F2,
    0x02D0, 0x82D5, 0x82DF, 0x02DA, 0x82CB, 0x02CE, 0x02C4, 0x82C1,
    0x8243, 0x0246, 0x024C, 0x8249, 0x0258, 0x825D, 0x8257, 0x0252,
    0x0270, 0x8275, 0x827F, 0x027A, 0x826B, 0x026E, 0x0264, 0x8261,
    0x0220, 0x8225, 0x822F, 0x022A, 0x823B, 0x023E, 0x0234, 0x8231,
    0x8213, 0x0216, 0x021C, 0x8219, 0x0208, 0x820D, 0x8207, 0x0202
  };
  for(j = 0; j < data_blk_size; j++) {
    i = ((unsigned short)(crc_accum >> 8) ^ data_blk_ptr[j]) & 0xFF;
    crc_accum = (crc_accum << 8) ^ crc_table[i];
  }
  return crc_accum;
}
