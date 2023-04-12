#include "stdafx.h"
#include "HidControl.h"
#include <Setupapi.h>
#include <math.h>

typedef enum
{
	HID_Out_ID =0,
	HID_Out_Address_H,
	HID_Out_Address_L,
	HID_Out_Length,
	HID_Out_Data,

}HIDOutputFormat;

typedef enum
{
	HID_In_ID =0,
	HID_In_Event,
	HID_In_Address_H =3,
	HID_In_Address_L,
	HID_In_Interrupt,
	HID_In_Data,

}HIDInputFormat;

typedef enum
{
	UART_INT =0x40,
	GPI_INT =0x20,
	SPI_Slave_INT =0x10,
	SPI_Master_INT =0x08,
	I2C_Slave_INT =0x04,
	I2C_Master_INT =0x02,
	IR_INT =0x01

}HIDInterruptType;

const BYTE OutputReport_DataSize =12;
const BYTE InputReport_DataSize =10;
const BYTE EndofRegister =0xFF;

typedef enum
{
	I2C_SLAVE=0,
	I2C_MAPL,
	I2C_MAPH,
	I2C_DATA,  //register 0x83 ~ 0x92(16 bytes)
	I2C_CTRL0 =0x13,
	I2C_CTRL1,
	I2C_CTRL2

}I2CFormat;

typedef enum
{
	Reg_I2C_SlaveDeviceAddr =0x80,
	Reg_I2C_SlaveAddr_L,
	Reg_I2C_SlaveAddr_H,
	Reg_I2C_Data,
	Reg_I2C_Control0 =0x93,
	Reg_I2C_Control1 =0x94,
	Reg_I2C_DownloadControl =0x95

}Register_I2C;

typedef enum
{
	Reg_ReportAddress =0xFF

}Register_Common;

DWORD __stdcall ReadThreadProc(LPVOID pObj )
{
	HidControl* p =NULL;
	p =(HidControl*) pObj;
	
	if(NULL!=p)	
	{
		p->ReadProc();
	}

	return 0;
}



HidControl::HidControl()
{
	m_hParentWnd =NULL;
	m_hDeviceHandle =NULL;					// device handle
	m_hReadHandle =NULL;	
	m_hDeviceEventObject =NULL;				// device event object					
	m_hReadThread =NULL;
	m_hEventObject =NULL;					// event object
	m_hStop =NULL;
	m_bSlaveDeviceAddress =0;
	m_EEPROMType =S24C32;


	m_hStop =CreateEvent( NULL, FALSE, FALSE, NULL );
	m_hI2CReadDone  =CreateEvent( NULL, TRUE, FALSE, NULL );

	InitializeCriticalSection( &m_CriticalSection );
}
HidControl::~HidControl()
{
	SAFE_CLOSE(m_hStop);
	SAFE_CLOSE(m_hI2CReadDone);

	::DeleteCriticalSection( &m_CriticalSection );
}

BOOL HidControl::SetHWND(HWND hwnd)
{
	if(!hwnd)	
		return FALSE;

	if(!IsWindow(hwnd))
		return FALSE;
	
	m_hParentWnd = hwnd;

	return TRUE;
}



//find hid device 
BOOL HidControl::OpenDevice( IN WORD VenderID, IN WORD ProductID)
{
	PHIDP_PREPARSED_DATA				PreparsedData =NULL;
	PSP_DEVICE_INTERFACE_DETAIL_DATA	detailData =NULL;
	HIDD_ATTRIBUTES						Attributes;
	SP_DEVICE_INTERFACE_DATA			devInfoData;
	BOOL								bLastDevice =FALSE, bDeviceDetected =FALSE;
	BOOL								bResult =FALSE;
	
	GUID								HidGuid =GUID_NULL;;
	HANDLE								hDevInfo =NULL;
	ULONG								Length =0, Required =0;
	int									MemberIndex =0;
	TCHAR								EventName[MAX_PATH];

	HidD_GetHidGuid (&HidGuid);	
	
	hDevInfo = SetupDiGetClassDevs(	&HidGuid, 
									 NULL, 
									 NULL, 
									 DIGCF_PRESENT | DIGCF_INTERFACEDEVICE);
		
	devInfoData.cbSize = sizeof (devInfoData);

	MemberIndex = 0;
	bLastDevice = FALSE;
	bDeviceDetected = FALSE;
	DWORD dwErrorCode =0;
	do
	{
		bResult = SetupDiEnumDeviceInterfaces 
			(hDevInfo, 
			 0, 
			 &HidGuid, 
			 MemberIndex, 
			 &devInfoData);

		if(bResult)
		{
			//A device has been detected, so get more information about it.
			bResult = SetupDiGetDeviceInterfaceDetail( hDevInfo, 
													  &devInfoData, 
													  NULL, 
												      0, 
													  &Length, 
													  NULL);

			
			detailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA) malloc (Length);
			
			
			detailData->cbSize = sizeof (SP_DEVICE_INTERFACE_DETAIL_DATA);

			//Call the function again, this time passing it the returned buffer size.
			bResult = SetupDiGetDeviceInterfaceDetail 
				(hDevInfo, 
				&devInfoData, 
				detailData, 
				Length, 
				&Required, 
				NULL);

			//Open a handle to the device.
			m_hDeviceHandle = CreateFile 
				(detailData->DevicePath, 
				GENERIC_READ | GENERIC_WRITE, 
				FILE_SHARE_READ | FILE_SHARE_WRITE, 
				NULL,
				OPEN_EXISTING, 
				FILE_FLAG_OVERLAPPED, 
				NULL);

			if (m_hDeviceEventObject == NULL)
			{
				time_t ltime;time(&ltime);
				
				memset( EventName, 0, sizeof(EventName) );
				_stprintf_s(EventName, MAX_PATH, TEXT("CM_HID_DEVICE_EVENT %d"), ltime);

				m_hDeviceEventObject = CreateEvent (
					NULL,
					TRUE,
					FALSE, 
					EventName);

				m_HIDDeviceOverlapped.hEvent = m_hDeviceEventObject;
				m_HIDDeviceOverlapped.Offset = 0;
				m_HIDDeviceOverlapped.OffsetHigh = 0;
			}

			Attributes.Size = sizeof (Attributes);

			bResult = HidD_GetAttributes 
				(m_hDeviceHandle, 
				&Attributes);
			
			if(bResult)
			{	
				if( (Attributes.VendorID == VenderID) &&
					(Attributes.ProductID == ProductID) )
				{
					//Both the Product and Vendor IDs match.
					bDeviceDetected = TRUE;

					//Get the m_Capabilities structure for the device.
					HidD_GetPreparsedData 
						(m_hDeviceHandle, 
						&PreparsedData);	

					HidP_GetCaps 
						(PreparsedData, 
						&m_Capabilities);
					
					HidD_FreePreparsedData (PreparsedData);

					// Create read handle
					m_hReadHandle = CreateFile 
						(detailData->DevicePath, 
						GENERIC_READ | GENERIC_WRITE, 
						FILE_SHARE_READ | FILE_SHARE_WRITE, 
						NULL,
						OPEN_EXISTING, 
						FILE_FLAG_OVERLAPPED,
						NULL);

					if (m_hEventObject == NULL)
					{
						time_t ltime;
						time(&ltime);

						memset( EventName, 0, sizeof(EventName) );
						_stprintf_s( EventName, MAX_PATH, TEXT("CM_HID_READ_EVENT %d"), ltime );

						m_hEventObject = CreateEvent(
							NULL,
							TRUE,
							FALSE,
							EventName);

						m_HIDOverlapped.hEvent = m_hEventObject;
						m_HIDOverlapped.Offset = 0;
						m_HIDOverlapped.OffsetHigh = 0;
					}
				}
				else
				{
					SAFE_CLOSE(m_hDeviceHandle);
				}

				free (detailData);
			}		

			//If we haven't found the device yet, and haven't tried every available device,
			//try the next one.
			MemberIndex = MemberIndex + 1;
		}
		else
		{
			dwErrorCode =GetLastError();
			if( ERROR_NO_MORE_ITEMS==dwErrorCode )
				bLastDevice=TRUE;
		}

	}while( (bLastDevice == FALSE) && (bDeviceDetected == FALSE) );

	SetupDiDestroyDeviceInfoList (hDevInfo);

	return bDeviceDetected;
}


// close device handle and stop read thread
BOOL HidControl::CloseDevice()
{
	if( NULL!=m_hStop )
		SetEvent(m_hStop);

	if( m_hDeviceHandle != NULL ) 
    {
        CancelIo(m_hDeviceHandle);
		SAFE_CLOSE(m_hDeviceHandle);
		
	}

    if( NULL!=m_hReadThread ) 
    {
		DWORD dwResult =0;
		dwResult =WaitForSingleObject(m_hReadThread, 2000);
		if( WAIT_OBJECT_0!=dwResult )
			TerminateThread( m_hReadThread, 0);
		SAFE_CLOSE(m_hReadThread);
	}
	

	SAFE_CLOSE(m_hReadHandle);

	SAFE_CLOSE(m_hDeviceEventObject);

	SAFE_CLOSE(m_hEventObject);

	return TRUE;
}

BOOL HidControl::SetEEPROMSize( IN EEPROM_IC type )
{
	if( type<S24C02 || type>S24C512 )
		return FALSE;;

	m_EEPROMType =type;

	return TRUE;
}

BOOL HidControl::StartReadThread()
{
	DWORD       ReadThreadID=0;

	if( NULL!=m_hReadThread )
		return FALSE;

	if( NULL==m_hReadThread )
	{
		m_hReadThread = CreateThread (
										NULL,
										0,
									    ReadThreadProc,
										this,
										0,
										&ReadThreadID);

	}

	return (NULL!=m_hReadThread);
}

//about definition of report 
//please reference CM65xx_HID_Format.docx
BOOL HidControl::RegisterAccess( IN BOOL bIsWrite, IN WORD wAddress, IN OUT BYTE* pData, IN BYTE bDataSize )
{
	BOOL bRet =FALSE;
	BYTE bOutputReport[16], bInputReport[16];
	BYTE bRemainderLen =bDataSize, bOffest =0;

	memset( bOutputReport, 0, sizeof(bOutputReport) );
	memset( bInputReport, 0, sizeof(bInputReport) );

	bOutputReport[HID_Out_ID] =1; //report id, always 1
	bInputReport[HID_Out_ID] =1;  //report id, always 1

	do
	{
		if( NULL==pData || 0==bDataSize )
			break;

		if( (EndofRegister+1)<(wAddress+bDataSize) ) 
			break;

		if(bIsWrite)
		{
			BYTE bWriteLen =0;
			
			while(bRemainderLen)
			{
				bWriteLen= (bRemainderLen/OutputReport_DataSize)? OutputReport_DataSize:bRemainderLen;
			
				bOutputReport[HID_Out_Address_H] =wAddress>>8;
				bOutputReport[HID_Out_Address_L] =wAddress & 0x00FF;
				bOutputReport[HID_Out_Length] =bWriteLen;
				memcpy( &bOutputReport[HID_Out_Data], &pData[bOffest], bWriteLen );
				bRet =Set_Output_Report( bOutputReport, sizeof(bOutputReport) );

				wAddress +=bWriteLen;
				bOffest +=bWriteLen;
				bRemainderLen -=bWriteLen;
			}
		}
		else
		{
			BYTE bReadLen =0;

			while(bRemainderLen)
			{
				bReadLen = (bRemainderLen/InputReport_DataSize)? InputReport_DataSize:bRemainderLen;

				if( !Set_Report_StartAddress(wAddress) )
					break;

				Sleep(10);

				if(Get_Intput_Report( bInputReport, sizeof(bInputReport) ))
				{
					if( (wAddress>>8)==bInputReport[HID_In_Address_H] &&
						(wAddress & 0x00FF)==bInputReport[HID_In_Address_L] )
					{
						memcpy( &pData[bOffest], &bInputReport[HID_In_Data], bReadLen);
						bRet =TRUE;
					}
				}

				bOffest +=bReadLen;
				wAddress +=bReadLen;
				bRemainderLen -=bReadLen;
			}
		}

	}while(FALSE);

	return bRet;
}

//about definition of I2C Register
//please reference CM65xx_I2C_Register.docx
BOOL HidControl::EEPROMAccess( IN BOOL bIsWrite, IN DWORD dwAddress, IN OUT BYTE* pData, IN DWORD dwDataSize )
{
	BOOL bRet =FALSE;
	
	do
	{
		BYTE	I2CCommand[21]; // register 0x80 ~ 0x94 (21 bytes)
		DWORD	dwPackSize		=1, dwOffset =0;
		DWORD   dwTotal = (dwAddress + dwDataSize),
			    dwRemainderLen =dwDataSize;

		if( NULL==pData || 0==dwDataSize )
			break;

		if( dwTotal>m_EEPROMType )
			break;

		if( !I2CRest() )
			break;


		if( !I2CMasterMode() )
			break;


		m_bSlaveDeviceAddress = (m_EEPROMType >= S24C32) ? 0xA8 : 0xA0;


		if(bIsWrite)
		{
			
			DWORD	dwWriteLen		= 0;
			BOOL    bWriteResult =FALSE;;
			
			memset( I2CCommand, 0, sizeof(I2CCommand));
	
			//0x9A: trigger to action + auto read + fast mode
			//using  16bit MAP when m_EEPROMType>= S24C32  
			//otherwise using 8bit MAP
			I2CCommand[I2C_CTRL1]	= 0x9A | (m_EEPROMType >= S24C32 ? 0x20 :0x00);

			do
			{	
				while(dwRemainderLen)
				{			
					
					if(m_EEPROMType >= S24C08)
					{
						if( 0==((dwAddress + dwOffset) % 16) )
						{
							dwPackSize	= 0x10;
						}
					}
					else
					{
						if( 0==((dwAddress + dwOffset) % 8) )
						{
							dwPackSize	= 0x08;
						}
					}
					

					dwWriteLen = (dwRemainderLen >= dwPackSize ? dwPackSize : dwRemainderLen);
					
					if(m_EEPROMType < S24C32)
					{
						I2CCommand[I2C_SLAVE]	= m_bSlaveDeviceAddress | (((dwAddress + dwOffset)/0x100)<<1);
						I2CCommand[I2C_MAPH]	= 0x00;
					}
					else
					{
						I2CCommand[I2C_SLAVE]	= m_bSlaveDeviceAddress;
						I2CCommand[I2C_MAPH]	= (dwAddress + dwOffset)>>8 & 0x00FF;
					}
					I2CCommand[I2C_MAPL]	= (dwAddress + dwOffset) & 0x00FF;					
					I2CCommand[I2C_CTRL0]	= dwWriteLen;

					
					memcpy( &I2CCommand[I2C_DATA], &pData[dwOffset], dwWriteLen );
			
					bWriteResult =I2CWrite( I2CCommand, sizeof(I2CCommand) );

					if( !bWriteResult )
						break;
						
					dwOffset +=dwWriteLen;
					dwRemainderLen  -=dwWriteLen;
				
				}

				bWriteResult? bRet = TRUE: bRet = FALSE;

			}while(FALSE);			

		}
		else
		{

			DWORD	dwReadLen =0, dwResult =0;
			BOOL    bReadResult =FALSE;;

			dwPackSize	= 0x10;
			memset( I2CCommand, 0, sizeof(I2CCommand));
	
			//0x9A: trigger to action + auto read + fast mode
			//using  16bit MAP when m_EEPROMType>= S24C32  
			//otherwise using 8bit MAP
			I2CCommand[I2C_CTRL1]	= 0x9A | (m_EEPROMType >= S24C32 ? 0x20 :0x00);

			do
			{	
				while( dwRemainderLen )
				{							

					dwReadLen = (dwRemainderLen >= dwPackSize ? dwPackSize : dwRemainderLen);
					
					if(m_EEPROMType < S24C32)
					{
						I2CCommand[I2C_SLAVE]	= m_bSlaveDeviceAddress | (((dwAddress + dwOffset)/0x100)<<1) | 0x01;
						I2CCommand[I2C_MAPH]	= 0x00;
					}
					else
					{
						I2CCommand[I2C_SLAVE]	= m_bSlaveDeviceAddress | 0x01;
						I2CCommand[I2C_MAPH]	= (dwAddress + dwOffset)>>8 & 0x00FF;
					}
					I2CCommand[I2C_MAPL]	= (dwAddress + dwOffset) & 0x00FF;					
					I2CCommand[I2C_CTRL0]	= dwReadLen;
			
					bReadResult =I2CRead( I2CCommand, sizeof(I2CCommand) );

					memcpy( &pData[dwOffset], &I2CCommand[I2C_DATA], dwReadLen );

					dwOffset +=dwReadLen;
					dwRemainderLen  -=dwReadLen;
				}

				bReadResult? bRet = TRUE: bRet = FALSE;

			}while(FALSE);	
		}

	}while(FALSE);
	
	return bRet;
}

//handle hid interrupt
void HidControl::ReadProc()
{
	BYTE InputReport[1024];
	DWORD NumberOfBytesRead =0;
	DWORD dwResult =0;
	BOOL bRet =FALSE;
	TCHAR TempStr[MAX_PATH]=_T("");

	if( NULL==m_hStop || NULL==m_hEventObject || NULL==m_hReadHandle )
		return;
	
	HANDLE hWaitArray[] ={ m_hStop, m_hEventObject };

	memset( InputReport, 0, sizeof(InputReport) );

	while( TRUE )
	{
		bRet = ReadFile (
			m_hReadHandle,
			InputReport,
			m_Capabilities.InputReportByteLength,
			&NumberOfBytesRead,
			(LPOVERLAPPED) &m_HIDOverlapped);

		dwResult =WaitForMultipleObjects( sizeof(hWaitArray)/sizeof(HANDLE),
			                      hWaitArray,
								  FALSE,
								  INFINITE );
		

		if( (WAIT_OBJECT_0+1)==dwResult )
		{
			ResetEvent (m_hEventObject);
			_stprintf_s(TempStr, MAX_PATH, TEXT("%2X %2X %2X %2X %2X %2X %2X %2X %2X %2X %2X %2X %2X %2X %2X %2X. \n"),
            InputReport[1], InputReport[2], InputReport[3], InputReport[4], InputReport[5], 
            InputReport[6], InputReport[7], InputReport[8], InputReport[9], InputReport[10], 
            InputReport[11], InputReport[12], InputReport[13], InputReport[14], InputReport[15], 
            InputReport[16]);

			//check interrupt type
			if( I2C_Master_INT & InputReport[HID_In_Interrupt] )
			{
				//check report address
				if( Reg_I2C_Data==InputReport[HID_In_Address_L] )
				{
					if(NULL!=m_hI2CReadDone)
						SetEvent(m_hI2CReadDone);
				}
			}
			
	#ifdef _DEBUG
			OutputDebugString(TempStr);
	#endif
		}
		else if( WAIT_OBJECT_0==dwResult ) 
		{
			//stop
			break;
		}
		else
		{
			//unexpected
		}
			
		
	}
}


BOOL HidControl::Set_Output_Report( IN const BYTE* pOutputReport, IN const DWORD& dwlen )
{
	if( NULL==m_hDeviceHandle || NULL==pOutputReport ) 
		return FALSE;

	EnterCriticalSection(&m_CriticalSection);

	DWORD	NumberOfBytesWritten =0;
	DWORD	dwResult =0;
	
	dwResult = WriteFile (
		m_hDeviceHandle, 
		pOutputReport, 
		dwlen, 
		&NumberOfBytesWritten, 
		&m_HIDDeviceOverlapped);
	
	dwResult = WaitForSingleObject (m_hDeviceEventObject, 500);
    if( WAIT_OBJECT_0!=dwResult ) 
		CancelIo(m_hDeviceHandle);

	ResetEvent (m_hDeviceEventObject);

	LeaveCriticalSection( &m_CriticalSection );


	return dwResult==WAIT_OBJECT_0;
}


BOOL HidControl::Get_Intput_Report(OUT BYTE *pInputReport , IN const DWORD& dwlen)
{
	if( NULL==m_hDeviceHandle || NULL==pInputReport )
		return FALSE;

	memset(pInputReport , 0 , dwlen);
	pInputReport[0] = 1;

	if( HidD_GetInputReport(m_hDeviceHandle, pInputReport, dwlen) )
	{   
		return TRUE;
	}
	else
		return FALSE;
}


BOOL HidControl::Set_Report_StartAddress( IN WORD wAddress )
{
	BOOL bRet =FALSE;

	BYTE bOutputReport[16];

	memset( bOutputReport, 0, sizeof(bOutputReport) );

	bOutputReport[HID_Out_ID] =1; //report id, always 1

	bOutputReport[HID_Out_Address_H] =0;
	bOutputReport[HID_Out_Address_L] =Reg_ReportAddress;
	bOutputReport[HID_Out_Length] =1;
	bOutputReport[HID_Out_Data] =wAddress & 0x00FF;
	bRet =Set_Output_Report( bOutputReport, sizeof(bOutputReport) );

	return bRet;
}


BOOL HidControl::I2CRest()
{
	BOOL bRet =FALSE;
	BYTE bOutputReport[16];

	memset( bOutputReport, 0, sizeof(bOutputReport) );

	bOutputReport[HID_Out_ID] =1; //always 1
	bOutputReport[HID_Out_Address_L] =Reg_I2C_Control1;
	bOutputReport[HID_Out_Length] =1;
	bOutputReport[HID_Out_Data] =0x50; //Reset I2C interface

	bRet =Set_Output_Report( bOutputReport, sizeof(bOutputReport) );

	return bRet;
}

BOOL HidControl::I2CMasterMode()
{
	BOOL bRet =FALSE;
	BYTE bOutputReport[16];

	memset( bOutputReport, 0, sizeof(bOutputReport) );

	bOutputReport[HID_Out_ID] =1; //always 1
	bOutputReport[HID_Out_Address_L] =Reg_I2C_DownloadControl;
	bOutputReport[HID_Out_Length] =1;
	bOutputReport[HID_Out_Data] =0x80; //I2C master mode

	bRet =Set_Output_Report( bOutputReport, sizeof(bOutputReport) );

	return bRet;
}

BOOL HidControl::I2CWrite( IN BYTE* pI2CData, IN BYTE bDataSiz )
{
	BOOL bRet =FALSE;
	BYTE bOutputReport[16], bTransportIndex =0, bTransportLen =bDataSiz;

	memset( bOutputReport, 0, sizeof(bOutputReport) );
	bOutputReport[HID_Out_ID] =1;

	do
	{
		if( NULL==pI2CData )
			break;

		//Each time Set_Output_Report function can transport  12 bytes.
		bTransportLen =(bTransportLen>OutputReport_DataSize) ? OutputReport_DataSize: bTransportLen;

		bOutputReport[HID_Out_Address_L]= Reg_I2C_SlaveDeviceAddr;
		bOutputReport[HID_Out_Length] =bTransportLen;
		memcpy( &bOutputReport[HID_Out_Data], pI2CData, bTransportLen );

		if( !Set_Output_Report( bOutputReport, sizeof(bOutputReport) ) )
			break;


		bTransportIndex +=bTransportLen;
		bTransportLen = bDataSiz-bTransportLen;
		bTransportLen =(bTransportLen>OutputReport_DataSize) ? OutputReport_DataSize: bTransportLen;

		bOutputReport[HID_Out_Address_L]= Reg_I2C_SlaveDeviceAddr+bTransportIndex;
		bOutputReport[HID_Out_Length] =bTransportLen;
		memcpy( &bOutputReport[HID_Out_Data], &pI2CData[bTransportIndex], bTransportLen );

		if( !Set_Output_Report( bOutputReport, sizeof(bOutputReport) ) )
			break;

		bRet =TRUE;

	}while(FALSE);

	return bRet;
}

BOOL HidControl::I2CRead( OUT BYTE* pI2CData, IN BYTE bDataSiz )
{
	BOOL bRet =FALSE;
	DWORD dwResult =0;

	do
	{
		if( NULL==m_hI2CReadDone || NULL==pI2CData )
			break;

		if( !Set_Report_StartAddress(Reg_I2C_Data) )
			break;


		bRet =I2CWrite( pI2CData, bDataSiz );

		ResetEvent( m_hI2CReadDone );
		dwResult =WaitForSingleObject( m_hI2CReadDone, 2000 );
		if( WAIT_OBJECT_0==dwResult )
		{
			bRet =RegisterAccess( FALSE, Reg_I2C_Data, &pI2CData[I2C_DATA], pI2CData[I2C_CTRL0] );		
		}

	}while(FALSE);

	return bRet;
}



