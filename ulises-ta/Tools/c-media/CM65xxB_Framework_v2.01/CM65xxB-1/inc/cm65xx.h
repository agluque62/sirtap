#ifndef __CM65xx_H__
#define __CM65xx_H__
#include "types.h"

#define MAX_BUFFER_SIZE		16
#define MAX_HID_REPORT_SIZE		16

#define pop_noise_1s		(4 * (0x01 << (MCU_CLK_SWITCH & 0x07)))

#define HID_REPORT_ID_GENEGRIC		0x01
#define HID_INPUT_REPORT_INT_IR		bmBIT0
#define HID_INPUT_REPORT_INT_I2C		bmBIT1
#define HID_INPUT_REPORT_INT_SPI		bmBIT3
#define HID_INPUT_REPORT_INT_UART		bmBIT6
#define HID_INPUT_REPORT_INT_GPIO		bmBIT5
#define HID_INPUT_REPORT_INT_I2C_SLAVE		bmBIT2
#define HID_INPUT_REPORT_INT_SPI_SLAVE		bmBIT4

#define DEFAULT_GPIO_RECORD_MUTE		0x03

typedef struct _AS_CONTROL_STRUCT
{
	BYTE attribute;		// bit7: invalid = 1, bit6: mono/stero, bit5~4: interface number, bit3~0: max alternate
	BYTE endpoint;		// endpoint number and direction
	void (*setFrequency)();
}AS_CONTROL_STRUCT;

typedef struct _AUDIO_CONTROL_STRUCT
{
	void (*featureVolume)();
	void (*featureMute)();
	void (*featureAgc)();
	void (*setSelector)();
	void (*recordMute)();
	BYTE code *pAcUnitTable;	
	AS_CONTROL_STRUCT asControl[2];
}AUDIO_CONTROL_STRUCT;	

typedef struct _DEVICE_CONTROL_STRUCT
{
	BYTE bAudioInterface;
	BYTE bHidInterface;		
}DEVICE_CONTROL_STRUCT;	

typedef struct _CONVERTIBLE_CONTROL_TABLE_STRUCT 
{
	DEVICE_CONTROL_STRUCT code	*pDeviceControl;
	AUDIO_CONTROL_STRUCT code	*pAudioControl;	
	void (*initialize)();
	void (*reset)();
	void (*configure)();
	void (*event)();
	void (*gpi)();
	void (*ir)();	
}CONVERTIBLE_CONTROL_TABLE_STRUCT;

typedef enum
{
	LANGID_S = 0,
	MANUFACTURE_S,
	PRODUCT_S,
	SERIAL_S,
	LYNC_S = 33,
	VERSION1_S = 50,
	VERSION2_S = 80
}STRING_DSCR;

typedef enum
{
	NEC = 0,
	RC5,
	RC6
}IR_MODULE;

#endif // __CM65xx_H__
