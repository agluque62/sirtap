#ifndef __USB_H__
#define __USB_H__
#include "types.h"
#include "registers.h"

#define DSCR_DEVICE				1		// Descriptor type: Device
#define DSCR_CONFIG				2		// Descriptor type: Configuration
#define DSCR_STRING				3		// Descriptor type: String
#define DSCR_INTRFC				4		// Descriptor type: Interface
#define DSCR_ENDPNT				5		// Descriptor type: Endpoint

#define DSCR_DEVICE_LEN       	18
#define DSCR_CONFIG_LEN       	9
#define DSCR_INTRFC_LEN      	9
#define DSCR_ENDPNT_LEN   		7
#define DSCR_HID_LEN			9

#define SC_GET_STATUS         	0x00   	// Setup command: Get Status
#define SC_CLEAR_FEATURE      	0x01   	// Setup command: Clear Feature
#define SC_RESERVED            	0x02   	// Setup command: Reserved
#define SC_SET_FEATURE         	0x03   	// Setup command: Set Feature
#define SC_SET_ADDRESS        	0x05   	// Setup command: Set Address
#define SC_GET_DESCRIPTOR      	0x06   	// Setup command: Get Descriptor
#define SC_SET_DESCRIPTOR      	0x07   	// Setup command: Set Descriptor
#define SC_GET_CONFIGURATION   	0x08   	// Setup command: Get Configuration
#define SC_SET_CONFIGURATION   	0x09   	// Setup command: Set Configuration
#define SC_GET_INTERFACE      	0x0A   	// Setup command: Get Interface
#define SC_SET_INTERFACE      	0x0B   	// Setup command: Set Interface
#define SC_SYNC_FRAME         	0x0C   	// Setup command: Sync Frame
#define SC_ANCHOR_LOAD         	0xA0   	// Setup command: Anchor load
   
#define GD_DEVICE          		0x01	// Get descriptor: Device
#define GD_CONFIGURATION   		0x02  	// Get descriptor: Configuration
#define GD_STRING          		0x03  	// Get descriptor: String
#define GD_INTERFACE       		0x04  	// Get descriptor: Interface
#define GD_ENDPOINT        		0x05  	// Get descriptor: Endpoint
#define GD_DEVICE_QUALIFIER 	0x06  	// Get descriptor: Device Qualifier
#define GD_OTHER_SPEED_CONFIGURATION 0x07  // Get descriptor: Other Configuration
#define GD_INTERFACE_POWER 		0x08  	// Get descriptor: Interface Power
#define GD_HID	            	0x21	// Get descriptor: HID
#define GD_HID_REPORT	        0x22	// Get descriptor: Report

#define GS_DEVICE          		0x80  	// Get Status: Device
#define GS_INTERFACE       		0x81  	// Get Status: Interface
#define GS_ENDPOINT        		0x82  	// Get Status: End Point

#define FT_DEVICE          		0x00  	// Feature: Device
#define FT_ENDPOINT        		0x02  	// Feature: End Point

#define RECIPIENT_MASK 			0x1F
#define RECIPIENT_DEVICE		0x00
#define RECIPIENT_INTERFACE 	0x01
#define RECIPIENT_ENDPOINT 		0x02

#define FEATURE_REMOTE_WAKEUP	0x01
#define FEATURE_EP_HALT			0x00
#define FEATURE_TEST_MODE		0x02		

#define HID_GET_REPORT   		0x01
#define HID_GET_IDLE     		0x02
#define HID_GET_PROTOCOL 		0x03
#define HID_SET_REPORT   		0x09
#define HID_SET_IDLE     		0x0A
#define HID_SET_PROTOCOL 		0x0B

#define USB_SETUP_DIR_MASK		bmBIT7

#define USB_SETUP_TYPE_MASK		(bmBIT5 | bmBIT6)	//Used to mask off request type
#define USB_SETUP_TYPE_STANDARD	0					//Standard Request
#define USB_SETUP_TYPE_CLASS	bmBIT5				//Class Request
#define USB_SETUP_TYPE_VENDOR	bmBIT6				//Vendor Request
#define USB_SETUP_TYPE_RESERVED (bmBIT5 | bmBIT6) 	//Reserved or illegal request

typedef enum
{
	NONE_STAGE,
	SETUP_STAGE,
	DATA_IN_STAGE,
	DATA_OUT_STAGE,
	STATUS_STAGE,
	STALL_STAGE
}USB_CONTROL_STATE;

typedef struct _USB_CONTROL_COMMAND_STRUCT
{
	BYTE bmRequestType;
	BYTE bRequest;
	WORD wValue;
	WORD wIndex;
	WORD wLength;
}USB_CONTROL_COMMAND_STRUCT; 	

typedef struct _USB_DEVICE_DSCR_STRUCT           
{
	BYTE bLength;				// Descriptor length ( = sizeof(DEVICEDSCR) )
	BYTE bDescriptorType;       // Decriptor type (Device = 1)
	WORD bcdUSB;   				// Specification Version (BCD) 
	BYTE bDeviceClass;   		// Device class
	BYTE bDeviceSubClass;      	// Device sub-class
	BYTE bDeviceProtocol;     	// Device sub-sub-class
	BYTE bMaxPacketSize0;      	// Maximum packet size for endpoint zero
	WORD idVendor;      		// Vendor ID
	WORD idProduct;				// Product ID
	WORD bcdDevice;      		// Product version ID
	BYTE iManufacture;      	// Manufacturer string index
	BYTE iProduct;      		// Product string index
	BYTE iSerialNumber;   		// Serial number string index
	BYTE bNumConfigurations;    // Number of configurations
}USB_DEVICE_DSCR_STRUCT;

typedef struct _USB_HID_DSCR_STRUCT
{
	BYTE bLength;				// Descriptor length ( = sizeof(DEVICEDSCR) )
	BYTE bDescriptorType;       // Decriptor type (Device = 1)	
	WORD bcdHID;
	BYTE bCountryCode;
	BYTE bNumDescriptors;
	BYTE bDescriptorType1;
	WORD wDescriptorLength;
}USB_HID_DSCR_STRUCT;

#define UsbEnableUsb()		(USB_GLOBAL_CTRL |= bmBIT7)
#define UsbResetAllEp()		(USB_EP_FIFO_RST = 0x0F)
#define UsbResetEpCtrl()		(USB_EP_FIFO_RST |= bmBIT0)
#define UsbResetEpInt()		(USB_EP_FIFO_RST |= bmBIT3)
#define UsbResetEpBlk4()		(USB_EP_FIFO_RST |= bmBIT4)
#define UsbResetEpBlk5()		(USB_EP_FIFO_RST |= bmBIT5)
#define UsbResetEpBlk7()		(USB_EP_FIFO_RST |= bmBIT1)
#define UsbResetEpBlk8()		(USB_EP_FIFO_RST |= bmBIT2)
#define UsbResetEpIsoInFb()		(USB_EP_FIFO_RST |= bmBIT6)
#define UsbSelectEp(ep)		(USB_EP_NUM = ep)

#define UsbStartEp()		(USB_EP_STAT_X |= bmBIT6)
#define UsbStopEp()		(USB_EP_STAT_X &= ~bmBIT6)

#define UsbEventResume()		(USB_GLOBAL_INT & bmBIT5)
#define UsbEventRst()		(USB_GLOBAL_INT & bmBIT4)
#define UsbEventSuspend()		(USB_GLOBAL_INT & bmBIT0)

#define UsbEventEpCtrl()		(USB_EP_INT & bmBIT0)
#define UsbEventEpInt()		(USB_EP_INT & bmBIT3)
#define UsbEventEpBlk4()		(USB_EP_INT & bmBIT4)
#define UsbEventEpBlk5()		(USB_EP_INT & bmBIT5)
#define UsbEventEpBlk7()		(USB_EP_INT & bmBIT1)
#define UsbEventEpBlk8()		(USB_EP_INT & bmBIT2)
#define UsbEventEpIsoInFb()		(USB_EP_INT & bmBIT6)
#define UsbClrEventEpBlk4()		(USB_EP_INT = bmBIT4)
#define UsbClrEventEpBlk5()		(USB_EP_INT = bmBIT5)
#define UsbClrEventEpBlk7()		(USB_EP_INT = bmBIT1)
#define UsbClrEventEpBlk8()		(USB_EP_INT = bmBIT2)
#define UsbClrEventEpIsoInFb()		(USB_EP_INT = bmBIT6)
#define UsbClrEventEpCtrl()		(USB_EP_INT = bmBIT0)
#define UsbClrEventEpInt()		(USB_EP_INT = bmBIT3)
#define UsbClrEventResume()		(USB_GLOBAL_INT &= ~bmBIT5)
#define UsbClrEventRst()		(USB_GLOBAL_INT &= ~bmBIT4)
#define UsbClrEventSuspend()		(USB_GLOBAL_INT &= ~bmBIT0)

#define UsbSetAddrEn()		(USB_GLOBAL_CTRL |= bmBIT0)
#define UsbSetAddress(addr)		(USB_ADDRESS = (bmBIT7 | addr)) 
#define UsbSetConfig()		(USB_GLOBAL_CTRL |= bmBIT1)
#define UsbClrConfig()		(USB_GLOBAL_CTRL &= ~bmBIT1)
#define UsbGetConfig()		(USB_GLOBAL_CTRL & bmBIT1)

#define UsbSetRemoteWakeup()		(USB_GLOBAL_CTRL |= bmBIT2)
#define UsbClrRemoteWakeup()		(USB_GLOBAL_CTRL &= ~bmBIT2)
#define UsbGetRemoteWakeup()		(USB_GLOBAL_CTRL & bmBIT2)

//lap {
#define UsbSetDir()		(USB_EP_STAT_X = ((USB_EP_STAT_X & 0xf0) | bmBIT7))
#define UsbSetStall()		(USB_EP_STAT_X = ((USB_EP_STAT_X & 0xf0) | bmBIT5))
#define UsbSetTxReady()		(USB_EP_STAT_X = ((USB_EP_STAT_X & 0xf0) | bmBIT4))
#define UsbSendStallComplete()		((USB_EP_STAT_X & 0xf8) & bmBIT3)
#define UsbRcvSetupPkt()		((USB_EP_STAT_X & 0xf4) & bmBIT2)
#define UsbRcvOutData()		((USB_EP_STAT_X & 0xf2) & bmBIT1)
#define UsbSendDataComplete()		((USB_EP_STAT_X & 0xf1) & bmBIT0)
#define UsbClrDir()		(USB_EP_STAT_X = ((USB_EP_STAT_X & 0xf0) & ~bmBIT7))
#define UsbClrStall()		(USB_EP_STAT_X = ((USB_EP_STAT_X & 0xf0) & ~bmBIT5))
#define UsbClrRxSetup()		(USB_EP_STAT_X = ((USB_EP_STAT_X & 0xf4) | bmBIT2))
#define UsbClrRxOutB0()		(USB_EP_STAT_X = ((USB_EP_STAT_X & 0xf2) | bmBIT1))
#define UsbClrTxCmpl()		(USB_EP_STAT_X = ((USB_EP_STAT_X & 0xf1) | bmBIT0))
//lap }

#define ENDPOINT_CTRL		0
#define ENDPOINT_ISO_OUT		1
#define ENDPOINT_ISO_IN		2
#define ENDPOINT_INT		3
#define ENDPOINT_BLK_N4		4
#define ENDPOINT_BLK_N5		5
#define ENDPOINT_ISO_IN_FB		6
#define ENDPOINT_BLK_N7		7
#define ENDPOINT_BLK_N8		8

#define HID_DSCR_LEN		(9 + 9 + 7)

#define REPORT_TYPE_INPUT		1
#define REPORT_TYPE_OUTPUT		2

#define FIFO_3B_ADDR		0x0B60
#define FIFO_2B_ADDR		0x0800

#endif // __USB_H__
