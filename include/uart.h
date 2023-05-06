#pragma once

//-----------------------------------------
// define baudrate const
// BAUD = 65536 - FOSC/3/BAUDRATE/M (1T:M=1; 12T:M=12)
// NOTE: (FOSC/3/BAUDRATE) must be greater then 98, (RECOMMEND GREATER THEN 110)

#define BAUD_1200_110592  0xF400                  // 1200bps @ 11.0592MHz
#define BAUD_2400_110592  0xFA00                  // 2400bps @ 11.0592MHz
#define BAUD_4800_110592  0xFD00                  // 4800bps @ 11.0592MHz
#define BAUD_9600_110592  0xFE80                    // 9600bps @ 11.0592MHz
#define BAUD_19200_110592  0xFF40                  // 19200bps @ 11.0592MHz
#define BAUD_38400_110592  0xFFA0                  // 38400bps @ 11.0592MHz

#define BAUD_1200_18432  0xEC00                  // 1200bps @ 18.432MHz
#define BAUD_2400_18432  0xF600                  // 2400bps @ 18.432MHz
#define BAUD_4800_18432  0xFB00                  // 4800bps @ 18.432MHz
#define BAUD_9600_18432  0xFD80                  // 9600bps @ 18.432MHz
#define BAUD_19200_18432  0xFEC0                  //19200bps @ 18.432MHz
#define BAUD_38400_18432  0xFF60                  //38400bps @ 18.432MHz

#define BAUD_1200_221184  0xE800                  // 1200bps @ 22.1184MHz
#define BAUD_2400_221184  0xF400                  // 2400bps @ 22.1184MHz
#define BAUD_4800_221184  0xFA00                  // 4800bps @ 22.1184MHz
#define BAUD_9600_221184  0xFD00                  // 9600bps @ 22.1184MHz
#define BAUD_19200_221184  0xFE80                  //19200bps @ 22.1184MHz
#define BAUD_38400_221184  0xFF40                  //38400bps @ 22.1184MHz
#define BAUD_57600_221184  0xFF80                  //57600bps @ 22.1184MHz

#include <stdint.h>
typedef unsigned char BYTE;
typedef unsigned int WORD;

extern BYTE TBUF, RBUF;
extern BYTE TDAT, RDAT;
extern BYTE TCNT, RCNT;
extern BYTE TBIT, RBIT;
extern BYTE TING, RING;
extern BYTE TEND, REND;

extern void uart_handle();
extern void uart_init(uint32_t baudrate);
extern void uart_send_byte(uint8_t data);
extern void uart_send_string(char* str);

extern BYTE uart_get_byte();
