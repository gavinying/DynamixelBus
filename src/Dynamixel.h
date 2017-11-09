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

#ifndef Dynamixel_H_
#define Dynamixel_H_

#ifdef __cplusplus
extern "C" {
#endif

#define BROADCAST_ADDRESS   (254)
#define MAXNUM_TX_PACKET    (255)
#define MAXNUM_RX_PACKET    (255)

/* Error type */
#define NO_ERROR        (0)
#define ERROR_TIMEOUT   (-1)
#define ERROR_OVERFLOW  (-2)
#define ERROR_INVALID   (-3)

/* Control mode */
#define MODE_WHEEL      (1)
#define MODE_JOINT      (2)

/* utility for value */
#define DXL_MAKEWORD(a, b)      ((unsigned short)(((unsigned char)(((unsigned long)(a)) & 0xff)) | ((unsigned short)((unsigned char)(((unsigned long)(b)) & 0xff))) << 8))
#define DXL_MAKEDWORD(a, b)     ((unsigned int)(((unsigned short)(((unsigned long)(a)) & 0xffff)) | ((unsigned int)((unsigned short)(((unsigned long)(b)) & 0xffff))) << 16))
#define DXL_LOWORD(l)           ((unsigned short)(((unsigned long)(l)) & 0xffff))
#define DXL_HIWORD(l)           ((unsigned short)((((unsigned long)(l)) >> 16) & 0xffff))
#define DXL_LOBYTE(w)           ((unsigned char)(((unsigned long)(w)) & 0xff))
#define DXL_HIBYTE(w)           ((unsigned char)((((unsigned long)(w)) >> 8) & 0xff))

#define LED_OFF         (0)
#define LED_RED         (1)
#define LED_GREEN       (2)
#define LED_YELLOW      (3)
#define LED_BLUE        (4)
#define LED_PINK        (5)
#define LED_CYAN        (6)
#define LED_WHITE       (7)

#ifdef __cplusplus
}
#endif

#endif
