#ifndef __CONFIG_H__
#define __CONFIG_H__

#define VENDOR_ID 0x0D8C
#define PRODUCT_ID 0x01C1
#ifdef VER
#define VERSION_ID VER
#else
#define VERSION_ID 0x0101
#endif

#define CHIP_ID 0x6502
#define CHIP_VERSION 0x0001     // 0x0001: ROM C     
#define EEPROM_PARAMETER 0x0000 //bit7: 1->Lync Basic Headset,0->not

#endif	// __CONFIG_H__

