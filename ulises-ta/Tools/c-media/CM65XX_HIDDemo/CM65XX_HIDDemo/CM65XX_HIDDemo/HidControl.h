#pragma once

extern "C" 
{ 
    #include "hidsdi.h"
}

typedef enum 
{
	S24C02=0x100,
	S24C04=0x200,
	S24C08=0x400,
	S24C16=0x800,
	S24C32=0x1000,
	S24C64=0x2000,
	S24C128=0x4000,
	S24C256=0x8000,
	S24C512=0x10000

}EEPROM_IC;

class HidControl  
{
public:
	
	HidControl();
	virtual ~HidControl();
	BOOL SetHWND( IN HWND hwnd );

	BOOL OpenDevice( IN WORD VenderID, IN WORD ProductID);
	BOOL CloseDevice();
	BOOL SetEEPROMSize( IN EEPROM_IC type);

	BOOL StartReadThread();

	BOOL RegisterAccess( IN BOOL bIsWrite, IN WORD wAddress, IN OUT BYTE* pData, IN BYTE bDataSize );
	BOOL EEPROMAccess( IN BOOL bIsWrite, IN DWORD dwAddress, IN OUT BYTE* pData, IN DWORD dwDataSize );
	void ReadProc();

private:
	BOOL Set_Output_Report( IN const BYTE* pOutputReport, IN const DWORD& dwlen );
	BOOL Get_Intput_Report( OUT BYTE *pInputReport , IN const DWORD& dwlen);
	BOOL Set_Report_StartAddress( IN WORD wAddress );

	BOOL I2CRest();
	BOOL I2CMasterMode();
	BOOL I2CWrite( IN BYTE* pI2CData, IN BYTE bDataSize );
	BOOL I2CRead( OUT BYTE* pI2CData, IN BYTE bDataSiz );

private:
	CRITICAL_SECTION m_CriticalSection;
	HWND		m_hParentWnd;
	
	HANDLE		m_hDeviceHandle;					// device handle
	HANDLE		m_hReadHandle;	
	HANDLE		m_hDeviceEventObject;				// device event object					
	HANDLE		m_hEventObject;					    // event object
	HANDLE		m_hReadThread;
	HANDLE      m_hStop;
	HANDLE		m_hI2CReadDone;		
	
	OVERLAPPED	m_HIDOverlapped, m_HIDDeviceOverlapped;

	HIDP_CAPS	m_Capabilities;
	
	BYTE        m_bSlaveDeviceAddress;
	EEPROM_IC   m_EEPROMType;
};

