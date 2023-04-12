#include <intrins.h>
#include "types.h"
#include "registers.h"
#include "cm65xx.h"
#include "audio.h"
#include "usb.h"
#include "config.h"

/*********************************************************************
* External library variable declaration
*********************************************************************/
extern BYTE idata g_bHidI2CSInfo[2];

/*********************************************************************
* External library function
*********************************************************************/
extern void M2MCopy(BYTE *dest, BYTE *src, BYTE size);
extern void C2MCopy(BYTE *dest, BYTE code *src, BYTE size);

/*********************************************************************
* Global/External variable definition
*********************************************************************/
BYTE *g_pbDataBuffer;

BYTE idata g_bHidInRegsInfo;
BYTE idata g_bInputReport[MAX_HID_REPORT_SIZE];
BYTE idata g_bOutputReport[MAX_HID_REPORT_SIZE];
BYTE idata g_bAlternateSetting[4];
BYTE idata s_bEpStatus;

BYTE code *s_pStringDscrTmp;

extern BOOL g_bmRemoteWakeupEn, g_bmSelfPower;

extern BYTE	g_bTemp;
extern BYTE	g_bIndex;
extern BYTE	g_bConfiguration;
extern BYTE	g_bDataBuffer[MAX_BUFFER_SIZE];

extern BYTE code *g_pbConfigDscr;
extern BYTE code *g_pHidDscr;
extern BYTE code *g_pHidReportDscr;
extern BYTE code *g_pProductStringDscr;
extern BYTE code *g_pManufactureStringDscr;
extern BYTE code DeviceDscr;
extern BYTE code LangIdStringDscr;
extern BYTE code ManufactureStringDscr;
extern BYTE code VersionStringDscr;

extern volatile BYTE xdata *g_pbXdataStart;

extern WORD	g_wDataLength, g_wDataOffset;

extern CONVERTIBLE_CONTROL_TABLE_STRUCT code *g_pControlTable;
extern USB_CONTROL_COMMAND_STRUCT g_UsbCtrlCmnd;

/*********************************************************************
* Global/External function definition
*********************************************************************/
extern BOOL TackleAudioControl(BOOL dataStage);
extern BOOL TackleSetAudioInterface();
extern BOOL TackleAudioStream(BOOL dataStage);

/*********************************************************************
* code area
*********************************************************************/
/**
 *
 */
void acquireDeviceDscrData()
{	
	if(g_wDataOffset == 0)
	{
		C2MCopy((BYTE *)g_bDataBuffer, (BYTE code *)(&DeviceDscr), MAX_BUFFER_SIZE);
		g_bDataBuffer[8] = LSB(VENDOR_ID);
		g_bDataBuffer[9] = MSB(VENDOR_ID);
		g_bDataBuffer[10] = LSB(PRODUCT_ID); 
		g_bDataBuffer[11] = MSB(PRODUCT_ID);
		g_bDataBuffer[12] = LSB(VERSION_ID); // Product version ID
		g_bDataBuffer[13] = MSB(VERSION_ID); // Product version ID
	}
	else
	{
		g_bDataBuffer[0] = 0;		// if the serial number is defined, this should be 3.
		g_bDataBuffer[1] = 1;
	}	 		

	g_pbDataBuffer = g_bDataBuffer;
}	

/**
 *
 */
void acquireConfigDscrData()
{
	if(g_wDataOffset == 0)
	{
		C2MCopy(g_bDataBuffer, g_pbConfigDscr, MAX_BUFFER_SIZE);
		g_pbDataBuffer = g_bDataBuffer;
	}	
	else
	{			
		g_pbDataBuffer = g_pbConfigDscr;
		g_pbDataBuffer += g_wDataOffset;				
	}
	
	if(g_wDataOffset == 0)
	{
		g_bDataBuffer[7] = bmBIT7 | (((BYTE)g_bmSelfPower) << 6) | (((BYTE)g_bmRemoteWakeupEn) << 5);
	}
}

/**
 *
 */
void acquireStringDscrData()
{
	g_pbDataBuffer = (BYTE *)s_pStringDscrTmp;
	g_pbDataBuffer += g_wDataOffset;
}

/**
 *
 */
void acquireHidReportDscrData()
{
	g_pbDataBuffer = g_pHidReportDscr;
	g_pbDataBuffer += g_wDataOffset;
}

/**
 *
 */
void acquireHidDscrData()
{
	C2MCopy((BYTE *)g_bDataBuffer, g_pHidDscr, DSCR_HID_LEN);
	g_pbDataBuffer = g_bDataBuffer;
}	

/**
 *
 */
BOOL tackleGetDescriptor(BOOL dataStage)
{
	switch(MSB(g_UsbCtrlCmnd.wValue)) 		
	{
		case GD_DEVICE:
			if(dataStage == FALSE)
			{
				g_wDataLength = DSCR_DEVICE_LEN;			
			}
			acquireDeviceDscrData();
		  break;
		
		case GD_CONFIGURATION:
			if(dataStage == FALSE)
			{
				g_wDataLength = (g_pbConfigDscr[3] << 8) | (g_pbConfigDscr[2]);						
			}
			acquireConfigDscrData();
		  break;	
		  
		case GD_STRING:  
		 	if(dataStage == FALSE)
		 	{
		 		switch(LSB(g_UsbCtrlCmnd.wValue))
		 		{
		 			case LANGID_S:
						s_pStringDscrTmp = (BYTE *)&LangIdStringDscr;	
						g_wDataLength = 4;
						break;
						
					case MANUFACTURE_S:
						s_pStringDscrTmp = 	(BYTE *)&ManufactureStringDscr;
						g_wDataLength = s_pStringDscrTmp[0];
						break;
						
					case PRODUCT_S:
						s_pStringDscrTmp = 	g_pProductStringDscr;
						g_wDataLength = s_pStringDscrTmp[0]; 
						break;

					case SERIAL_S:
						return FALSE;
						break;
						
					case VERSION1_S:
					case VERSION2_S:
						s_pStringDscrTmp = 	(BYTE *)&VersionStringDscr;
						g_wDataLength = s_pStringDscrTmp[0];
						break;
						
					default:
						return FALSE;
						break;
		 		}
			}	
			acquireStringDscrData();
		  break;		

		case GD_HID:
			if(dataStage == FALSE)
			{
				g_wDataLength = DSCR_HID_LEN;			
			}
			acquireHidDscrData();			
		  break;			  
		  
		case GD_HID_REPORT:
		 	if(dataStage == FALSE)
		 	{
				g_wDataLength = ((WORD)(g_pHidDscr[8]) << 8) | (WORD)(g_pHidDscr[7]);
			}
			acquireHidReportDscrData();	
		  break;		  		  		  	
		  
		default:
			return FALSE;
	}

	return TRUE;
}

/**
 *
 */
BOOL tackleSetAddress()
{
	UsbSetAddress(LSB(g_UsbCtrlCmnd.wValue));
	
	UsbResetEpCtrl();
	UsbSetAddrEn();
	
	return TRUE;
}	

/**
 *
 */
void epIntClearHalt()
{
	UsbSelectEp(ENDPOINT_INT);
	UsbClrStall();
	UsbResetEpInt();
	UsbSelectEp(ENDPOINT_CTRL);
	s_bEpStatus = 0;	
}	

/**
 *
 */
BOOL tackleSetConfiguration()
{
	g_bConfiguration = LSB(g_UsbCtrlCmnd.wValue);
	
	if(g_bConfiguration)
	{
		UsbSetConfig();		
		
		g_pControlTable->configure();		
	}
	else
	{
		UsbClrConfig();
	}
		
	if(s_bEpStatus & bmBIT7)
	{
		epIntClearHalt();
	}
	
	return TRUE;
}	

/**
 *
 */
BOOL tackleSetInterface()
{	
	if(UsbGetConfig() == 0)
	{
		return FALSE;
	}

	if(s_bEpStatus & bmBIT7)
	{
		epIntClearHalt();
	}

	g_bTemp = g_pControlTable->pDeviceControl->bHidInterface;
		
	if(LSB(g_UsbCtrlCmnd.wIndex) && (LSB(g_UsbCtrlCmnd.wIndex) < g_bTemp))
	{
		if(TackleSetAudioInterface() == FALSE)
		{
			return FALSE;
		}				
	}	 		
		
	g_bAlternateSetting[LSB(g_UsbCtrlCmnd.wIndex)] = LSB(g_UsbCtrlCmnd.wValue);	

	return TRUE;
}

/**
 *
 */
BOOL tackleGetConfiguration()
{	
	g_wDataLength = 1;
	g_pbDataBuffer = &g_bConfiguration;

	return TRUE;	
}	

/**
 *
 */
BOOL tackleGetInterface()
{
	if(UsbGetConfig() == 0)
	{
		return FALSE;	
	}

	g_wDataLength = 1;

	g_pbDataBuffer = &(g_bAlternateSetting[LSB(g_UsbCtrlCmnd.wIndex)]);		
	
	return TRUE;	
}

/**
 *
 */
BOOL tackleSetFeature()
{
	g_bTemp = g_UsbCtrlCmnd.bmRequestType & RECIPIENT_MASK;	
	if(g_bTemp == RECIPIENT_DEVICE)
	{
		if((LSB(g_UsbCtrlCmnd.wValue)) == FEATURE_REMOTE_WAKEUP) 
		{
			if(g_bmRemoteWakeupEn == TRUE)
			{
				UsbSetRemoteWakeup();	
			}
						
			return TRUE;			
		}	
	}
	else if(g_bTemp == RECIPIENT_ENDPOINT)
	{
		if(LSB(g_UsbCtrlCmnd.wValue) == FEATURE_EP_HALT) 
		{
			g_bIndex = (LSB(g_UsbCtrlCmnd.wIndex)) & 0x7F;
			if(g_bIndex == 7)
			{
				UsbSelectEp(ENDPOINT_INT);
				UsbSetStall();
				UsbSelectEp(ENDPOINT_CTRL);
				s_bEpStatus = s_bEpStatus | bmBIT7;
				
				return TRUE;		
			}
			else if(g_bIndex == 0)
			{
				return TRUE;		
			}
		}
	}	

	return FALSE;
}	

/**
 *
 */
BOOL tackleClearFeature()
{
	g_bTemp = g_UsbCtrlCmnd.bmRequestType & RECIPIENT_MASK;	
	if(g_bTemp == RECIPIENT_DEVICE)
	{
		if((LSB(g_UsbCtrlCmnd.wValue)) == FEATURE_REMOTE_WAKEUP) 
		{
			if(g_bmRemoteWakeupEn)
			{
				UsbClrRemoteWakeup();
			}
			
			return TRUE;				
		}	
	}
	else if(g_bTemp == RECIPIENT_ENDPOINT)
	{
		if((LSB(g_UsbCtrlCmnd.wValue)) == FEATURE_EP_HALT) 
		{
			g_bIndex = (LSB(g_UsbCtrlCmnd.wIndex)) & 0x7F;
			if(g_bIndex == 7)
			{
				epIntClearHalt();
				
				return TRUE;		
			}
			else if(g_bIndex == 0)
			{
				return TRUE;		
			}
		}
	}	

	return FALSE;
}	

/**
 *
 */
BOOL tackleGetStatus()
{
	switch(g_UsbCtrlCmnd.bmRequestType & RECIPIENT_MASK)
	{
		case RECIPIENT_DEVICE:
		  g_bDataBuffer[0] = g_bmSelfPower;
		  if(UsbGetRemoteWakeup())
		  {
		  	g_bDataBuffer[0] |= bmBIT1;
		  }		  	
		  break;
		  
		case RECIPIENT_INTERFACE:
			g_bDataBuffer[0] = 0x00;
		  break;
		  
		case RECIPIENT_ENDPOINT:
			g_bTemp = bmBIT0 << ((LSB(g_UsbCtrlCmnd.wIndex)) & 0x7F);
		  g_bDataBuffer[0] = (s_bEpStatus & g_bTemp) ? 1 : 0;
		  break;
		  
		default:
		  return FALSE;  
	}		
	
	g_bDataBuffer[1] = 0x00;	
	g_wDataLength = 2;
	g_pbDataBuffer = g_bDataBuffer;	
	
	return TRUE;		
}	


/**
 *
 */
BOOL tackleHidGetReport()
{
	switch(MSB(g_UsbCtrlCmnd.wValue))
	{
		case REPORT_TYPE_INPUT:
			if(LSB(g_UsbCtrlCmnd.wValue) == HID_REPORT_ID_GENEGRIC)
			{
				g_bInputReport[0] = HID_REPORT_ID_GENEGRIC;
				g_bInputReport[3] = CTRL_REG_H_ADDR;
				g_bInputReport[4] = CTRL_REG_L_ADDR;
				
				g_pbXdataStart = (BYTE xdata *)((WORD)((CTRL_REG_H_ADDR << 8) | CTRL_REG_L_ADDR));
				for(g_bIndex = 0; g_bIndex < MIN(g_bHidInRegsInfo, (MAX_HID_REPORT_SIZE - 6)); g_bIndex++)
				{
					g_bInputReport[6 + g_bIndex] = g_pbXdataStart[g_bIndex]; 	
				}
				
				M2MCopy(g_bDataBuffer, g_bInputReport, MAX_HID_REPORT_SIZE);	
			}					
			else
			{
				return FALSE;  	  
		  	}
		  break;
	
		case REPORT_TYPE_OUTPUT:
			M2MCopy(g_bDataBuffer, g_bOutputReport, MAX_HID_REPORT_SIZE);
		  break;
		  
		default:
		  return FALSE;  
	}

	g_wDataLength = MAX_HID_REPORT_SIZE;
	g_pbDataBuffer = g_bDataBuffer;	
	
	return TRUE;
}

/**
 *
 */
BOOL tackleHidSetReport()
{	
	switch(MSB(g_UsbCtrlCmnd.wValue))
	{
		case REPORT_TYPE_INPUT:
		  break;
	
		case REPORT_TYPE_OUTPUT:		
		  break;
		
		default:
		  return FALSE;  
	}
	
	g_wDataLength = MAX_HID_REPORT_SIZE;
	g_pbDataBuffer = g_bDataBuffer;	
	
	return TRUE;
}

/**
 *
 */
BOOL tackleHidSetReportData()
{
	switch(MSB(g_UsbCtrlCmnd.wValue))
	{
		case REPORT_TYPE_INPUT:
			M2MCopy(g_bInputReport, (BYTE code *)g_bDataBuffer, MAX_HID_REPORT_SIZE);
			break;
	
		case REPORT_TYPE_OUTPUT:
			if(g_bDataBuffer[0] == HID_REPORT_ID_GENEGRIC)
			{
				if((g_bDataBuffer[1] == 0x00) && (g_bDataBuffer[2] == 0xFE))
				{
					g_bHidInRegsInfo = MIN(g_bDataBuffer[3], (MAX_HID_REPORT_SIZE - 6));
					CTRL_REG_H_ADDR = g_bDataBuffer[4];
					CTRL_REG_L_ADDR = g_bDataBuffer[5];
				}	
				else
				{
					g_pbXdataStart = (BYTE xdata *)((WORD)((g_bDataBuffer[1] << 8) | g_bDataBuffer[2]));
					g_bTemp = MIN(g_bDataBuffer[3], (MAX_HID_REPORT_SIZE - 4));	
					for(g_bIndex = 0; g_bIndex < g_bTemp; g_bIndex++)
					{
						g_pbXdataStart[g_bIndex] = g_bDataBuffer[4 + g_bIndex];
					}
				}

				M2MCopy(g_bOutputReport, (BYTE code *)g_bDataBuffer, MAX_HID_REPORT_SIZE);
			}
			else
			{
				return FALSE;  				
			}
			break;
	
		default:
			return FALSE;  
	}
	
	return TRUE;
}

/**
 *
 */
BOOL tackleClassCommand(BOOL dataStage)
{	
	g_bTemp = g_UsbCtrlCmnd.bmRequestType & RECIPIENT_MASK;
	
	if(g_bTemp == RECIPIENT_INTERFACE)
	{
		g_bTemp = LSB(g_UsbCtrlCmnd.wIndex);
	
		if(g_bTemp == 0)
		{			
			return TackleAudioControl(dataStage);						
		}
		else if(g_bTemp == (g_pControlTable->pDeviceControl->bHidInterface))
		{
			if(dataStage == FALSE)
			{
				switch(g_UsbCtrlCmnd.bRequest)
				{
					case HID_GET_REPORT:						
					  return tackleHidGetReport();
		
					case HID_SET_REPORT:						
					  return tackleHidSetReport();
		
					case HID_SET_IDLE:
						g_wDataLength = 0;
					  return TRUE;
				}	
			}
			else
			{
				switch(g_UsbCtrlCmnd.bRequest)
				{
					case HID_SET_REPORT:
					  return tackleHidSetReportData();
				}
 			}							
		}		
		else
		{
			return FALSE;
		}
	}	
	else if(g_bTemp == RECIPIENT_ENDPOINT)
	{
		return TackleAudioStream(dataStage);	
	}	
	
	return FALSE;	
}	

/**
 *
 */
BOOL tackleVendorCommand(BOOL dataStage)
{			
	if(dataStage == FALSE)
	{
		g_pbDataBuffer = g_bDataBuffer;	
		if(g_UsbCtrlCmnd.wLength <= MAX_BUFFER_SIZE)
		{
			g_wDataLength = g_UsbCtrlCmnd.wLength;
		}
		else 
		{
			return FALSE;
		}
		
		if(g_UsbCtrlCmnd.bmRequestType & USB_SETUP_DIR_MASK)
		{
			if(g_UsbCtrlCmnd.bRequest == 0x02)			// Register read
			{								
				g_pbXdataStart = (BYTE xdata *)(LSB(g_UsbCtrlCmnd.wValue));	
				for(g_bIndex = 0; g_bIndex < g_wDataLength; g_bIndex++)
				{
					g_bDataBuffer[g_bIndex] = g_pbXdataStart[g_bIndex];
				}
				
				return TRUE;
			}		
		}
		else
		{	
			if(g_UsbCtrlCmnd.bRequest == 0x01)			// Register write						
			{
				return TRUE;				
			}
		}
	}
	else
	{						
		if(g_UsbCtrlCmnd.bRequest == 0x01)			// Register write
		{
			g_pbXdataStart = (BYTE xdata *)(LSB(g_UsbCtrlCmnd.wValue));
			for(g_bIndex = 0; g_bIndex < g_wDataLength; g_bIndex++)
			{
				g_pbXdataStart[g_bIndex] = g_bDataBuffer[g_bIndex];							
			}
				
			return TRUE;	
		}							
	}		
	
	return FALSE;
}	

/**
 *
 */
BOOL TackleControlRequest(BOOL dataStage)
{	
	g_bTemp = g_UsbCtrlCmnd.bmRequestType & USB_SETUP_TYPE_MASK;
	
	if(g_bTemp == USB_SETUP_TYPE_STANDARD)
	{				
		if(dataStage == FALSE)
		{
			g_wDataOffset = 0;						
			switch(g_UsbCtrlCmnd.bRequest)
			{
				case SC_GET_DESCRIPTOR:							
				  return tackleGetDescriptor(FALSE);				
				
				case SC_SET_ADDRESS:
				  return tackleSetAddress();
				
				case SC_SET_INTERFACE:
				  return tackleSetInterface();
				  				
				case SC_SET_CONFIGURATION:
				  return tackleSetConfiguration();
			
				case SC_GET_INTERFACE:
				  return tackleGetInterface();
			
				case SC_GET_CONFIGURATION:
				  return tackleGetConfiguration();
			
				case SC_SET_FEATURE:
				  return tackleSetFeature();
				  
				case SC_CLEAR_FEATURE:
				  return tackleClearFeature();
				  
				case SC_GET_STATUS:
				  return tackleGetStatus();  
			}
		}
		else
		{	
			switch(g_UsbCtrlCmnd.bRequest)
			{				
				case SC_GET_DESCRIPTOR:
				  return tackleGetDescriptor(TRUE);
			}			
		}	
	}	
	else if(g_bTemp == USB_SETUP_TYPE_CLASS)
	{
		if(dataStage == FALSE)
			g_wDataOffset = 0;
					
		return tackleClassCommand(dataStage);
	}
	else if(g_bTemp == USB_SETUP_TYPE_VENDOR)
	{
		return tackleVendorCommand(dataStage);
	}	
	
	return FALSE;
}	