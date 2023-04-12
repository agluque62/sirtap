#ifndef __TYPES_H__
#define __TYPES_H__

#define TRUE  	1
#define FALSE 	0

#define NULL	0

#define bmBIT0	0x01
#define bmBIT1  0x02
#define bmBIT2  0x04
#define bmBIT3  0x08
#define bmBIT4  0x10
#define bmBIT5  0x20
#define bmBIT6  0x40
#define bmBIT7  0x80

#define bmBIT8      0x0100
#define bmBIT9      0x0200
#define bmBIT10     0x0400
#define bmBIT11     0x0800
#define bmBIT12     0x1000
#define bmBIT13     0x2000
#define bmBIT14     0x4000
#define bmBIT15     0x8000

typedef signed char SBYTE;
typedef signed short SWORD;

typedef unsigned char  	BYTE;
typedef unsigned long  	DWORD;
typedef unsigned short 	WORD;

typedef bit				BIT;
typedef bit				BOOL;

#define MSB(word) 		(BYTE)(((WORD)(word) >> 8) & 0x00ff)
#define LSB(word) 		(BYTE)((WORD)(word))

#define HI_NIBBLE(byte)	(byte >> 4)
#define LO_NIBBLE(byte)	(byte & 0x0F)

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define D_VOL(byte1, byte2)		(((SBYTE)(byte2) >= 0) ? (byte1 + byte2) : (byte1 - (0 - byte2)))
#define A_VOL(byte1, byte2)		(((SBYTE)(byte2) >= 0) ? (byte1 - byte2) : (byte1 + (0 - byte2)))

#define EX_INT0_EN		0x01
#define EX_TIMER0_EN	0x02
#define EX_INT1_EN		0x04

#endif // __TYPES_H__