#include <intrins.h>
#include "types.h"
#include "registers.h"
#include "cm65xx.h"
#include "audio.h"
#include "usb.h"

/*********************************************************************
* External library variable declaration
*********************************************************************/

/*********************************************************************
* Global/External variable definition
*********************************************************************/
WORD g_wDataLength, g_wDataOffset;

USB_CONTROL_STATE g_UsbCtrlState;
USB_CONTROL_COMMAND_STRUCT g_UsbCtrlCmnd;

extern BYTE g_bIndex;
extern BYTE *g_pbDataBuffer;

extern BYTE idata g_bInputReport[MAX_HID_REPORT_SIZE];

/**
 *******************************************************************
 * code area
 ********************************************************************
 */
BOOL TackleControlRequest(BOOL dataStage);

/**
 *
 */
void HandleUsbCtrlTransfer()
{
	BYTE len;	
		
	// Setup Stage	
	UsbSelectEp(ENDPOINT_CTRL);
	if(UsbRcvSetupPkt())
	{
		UsbResetEpCtrl();
		
		g_UsbCtrlState = SETUP_STAGE;
		
		g_UsbCtrlCmnd.bmRequestType = USB_EP_DATA_X;	
		g_UsbCtrlCmnd.bRequest = USB_EP_DATA_X;
		g_UsbCtrlCmnd.wValue = USB_EP_DATA_X;
		g_UsbCtrlCmnd.wValue |= ((WORD)USB_EP_DATA_X << 8);
		g_UsbCtrlCmnd.wIndex = USB_EP_DATA_X;
		g_UsbCtrlCmnd.wIndex |= ((WORD)USB_EP_DATA_X << 8);
		g_UsbCtrlCmnd.wLength = USB_EP_DATA_X;
		g_UsbCtrlCmnd.wLength |= ((WORD)USB_EP_DATA_X << 8);	
	
		if(TackleControlRequest(FALSE) == TRUE)
		{
			g_wDataLength = MIN(g_UsbCtrlCmnd.wLength, g_wDataLength);
			
			if(!g_wDataLength)
				g_UsbCtrlState = STATUS_STAGE;
			else 
			{				
				if(g_UsbCtrlCmnd.bmRequestType & USB_SETUP_DIR_MASK)	// Device to Host
					g_UsbCtrlState = DATA_IN_STAGE;							
				else
					g_UsbCtrlState = DATA_OUT_STAGE;
			
				UsbSetDir();	
			}											
		}
		else			
		{		
			g_UsbCtrlState = STALL_STAGE;
			UsbSetStall();				
		}	
		
		UsbClrRxSetup();	
		UsbClrEventEpCtrl();
	}

	// DataOut Stage
	UsbSelectEp(ENDPOINT_CTRL);
	if((g_UsbCtrlState == DATA_OUT_STAGE) && UsbRcvOutData())
	{
		if(g_wDataLength > 16)
			len = 16;
		else
			len = g_wDataLength;	
			
		UsbResetEpCtrl();
			
		for(g_bIndex = 0; g_bIndex < len; ++g_bIndex)
		{
			g_pbDataBuffer[g_bIndex] = USB_EP_DATA_X;
		}						
		
		UsbClrDir();			
		UsbClrRxOutB0();	
		UsbClrEventEpCtrl();
		
		if(TackleControlRequest(TRUE) == TRUE)
		{
			g_wDataLength -= len;
			g_wDataOffset += len;				
						
			if(g_wDataLength == 0)
				g_UsbCtrlState = STATUS_STAGE;
		}
		else
		{		
			g_UsbCtrlState = STALL_STAGE;
			UsbSetStall();				
		}	
	}	

	// DataIn Stage
	UsbSelectEp(ENDPOINT_CTRL);
	if(g_UsbCtrlState == DATA_IN_STAGE)
	{
		if(g_wDataLength > 16)
		{
			len = 16;
		}
		else
		{
			len = g_wDataLength;	
		}
			
		for(g_bIndex = 0; g_bIndex < len; ++g_bIndex)
		{	
			USB_EP_DATA_X = g_pbDataBuffer[g_bIndex];
		}
				
		UsbSetTxReady();
		
		g_wDataLength -= len;
		g_wDataOffset += len;
		
		if(!g_wDataLength)
		{
			while(1)
			{
				if(UsbSendDataComplete())
				{
					UsbClrTxCmpl();
					UsbClrEventEpCtrl();
					
					if(len == 16)
					{				
						 // send zero length packet				
						UsbResetEpCtrl();
						UsbSetTxReady();
							    
						while(1)
						{
							if(UsbSendDataComplete())
							{
								UsbClrTxCmpl();
								UsbClrEventEpCtrl();
								g_UsbCtrlState = STATUS_STAGE;
								break;
							}
										    
							if(UsbRcvSetupPkt())
								break;
											    
							if(UsbRcvOutData())
							{
								UsbClrDir();
								UsbClrRxOutB0();
								UsbClrEventEpCtrl();
								g_UsbCtrlState = NONE_STAGE;  
								break;
							}	
						}
					}
					else
					{
						g_UsbCtrlState = STATUS_STAGE;
					}
					
					break;  
				}
				
				if(UsbRcvOutData())
				{
  				UsbClrDir();
  				UsbClrRxOutB0();
					UsbClrEventEpCtrl();
  				g_UsbCtrlState = NONE_STAGE;  
					break;
				}	
				
				if(UsbRcvSetupPkt())
					break;
			}
		}
		else
		{						
			TackleControlRequest(TRUE);				
			
			while(!(UsbSendDataComplete()))
			{
				if(UsbRcvSetupPkt())
					break;					
			}		
			UsbClrTxCmpl();
		}
	}	
	
	// Status Stage
	UsbSelectEp(ENDPOINT_CTRL);
	if(g_UsbCtrlState == STATUS_STAGE)
	{		
		if(g_UsbCtrlCmnd.bmRequestType & USB_SETUP_DIR_MASK)
		{
			while(!(UsbRcvOutData()))
			{
				if(UsbRcvSetupPkt())
					break;	
			}
			UsbClrDir();
			UsbClrRxOutB0();	
			UsbClrEventEpCtrl();
		}
		else
		{		
			UsbSetTxReady();
			while(!(UsbSendDataComplete()))
			{
				if(UsbRcvSetupPkt())
					break;					
			}	
			UsbClrTxCmpl();
			UsbClrEventEpCtrl();
		}
		
		g_UsbCtrlState = NONE_STAGE;
	}
	
	// STALL
	UsbSelectEp(ENDPOINT_CTRL);
	if(g_UsbCtrlState == STALL_STAGE)
	{
		while(!(UsbSendStallComplete()))
		{
			if(UsbRcvSetupPkt())
				break;			
		}		
		UsbClrStall();		
		g_UsbCtrlState = NONE_STAGE;
	}	
}

/**
 *
 */
void SubmitUsbIntTransfer()
{	
	UsbSelectEp(ENDPOINT_INT);		

	UsbResetEpInt();
	
	for(g_bIndex = 0; g_bIndex < MAX_HID_REPORT_SIZE; ++g_bIndex)
	{	
		USB_EP_DATA_X = g_bInputReport[g_bIndex];
	}
		
	UsbSetTxReady();		
}	

/**
 *
 */
void HandleUsbIntTransfer()
{	
	UsbSelectEp(ENDPOINT_INT);	

	if(UsbSendDataComplete())
	{
		UsbClrTxCmpl();
	}			
}
