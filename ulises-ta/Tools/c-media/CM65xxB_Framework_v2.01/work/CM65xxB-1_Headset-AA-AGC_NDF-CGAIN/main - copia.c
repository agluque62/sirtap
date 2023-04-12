//#define ALLOCATE_EXTERN

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
extern BOOL g_bmUsbPuReset;

extern volatile BYTE xdata I2CM_DEV_ADDR;
extern volatile BYTE xdata I2CM_MAP_ADDR[2];
extern volatile BYTE xdata I2CM_DATA_BUF[16];
extern volatile BYTE xdata I2CM_DATA_LEN;
extern volatile BYTE xdata I2CM_16BIT_MODE;

/*********************************************************************
* External library function
*********************************************************************/
extern BYTE InternSpiReadByte(BYTE addr);

extern void RefreshWdt();
extern void POR_Init();
extern void GPIO_Init(WORD dir_mask, WORD int_en, BOOL remote_wakeup_en);
extern void LED_Config(BYTE led1, BYTE led2, BYTE led3);
extern void LED1_Switch(BOOL open);
extern void UartBaudRateInit();
extern void HubInSuspend();
extern void InternSpiWriteByte(BYTE addr, BYTE value);
extern void AntiPopProcedure();
extern void AdcCodecReset(BOOL ext_i2s, BOOL slv, BOOL mclk_out, BOOL dsp_mode);
extern void DacCodecReset(BOOL ext_i2s, BOOL slv, BOOL mclk_out, BOOL dsp_mode);
extern void PDSW_Control(BOOL en);
extern void GpioPuPdSetting(WORD dir_mask, BOOL r_type);
extern void MemZero(BYTE *dest, BYTE size);

/*********************************************************************
* Global/External variable definition
*********************************************************************/
// variables inside ROM
BYTE g_bExtIntEnable	_at_ 0x0067;
/*
2:ext int1 ISR enable
1:ext timer0 ISR enable
0:ext int0 ISR enable
*/

/*
 ********************************************************************
 * Nucleo...
 ********************************************************************
 */
#ifdef __NUCLEODF__
	extern BYTE code ProductStringDscr_01, ProductStringDscr_02, ProductStringDscr_03, ProductStringDscr_04;
	#define LED0	0x10	// bit 4
	#define LED1	0x20	// bit 5
	void InitProductName();
	void leds(BYTE nled, BOOL val);
	void setDefaultGains(int slave);

	WORD _wgpi;				// Marca el Slave (0,1,2,3).

#endif

BOOL 	g_bmRemoteWakeupEn;
BOOL 	g_bmSpdifOutEn;
BOOL 	g_bmSelfPower;
BOOL 	g_bmAdcHPSEn;
BOOL	g_bmAgcEn;				// AGL. Enable Global del AGC...
BOOL	g_bmCurrentAgc;			// AGL. Activa o Inactiva AGC... Se inicializa a TRUE. 
BOOL	g_bmAaPathEn;


volatile BOOL g_bmUsbResume;
volatile BOOL g_bmGpiRequest;

BYTE g_bTemp, g_bTempA, g_bTempB, g_bIndex, g_bIndex2;
BYTE g_bConfiguration, g_bTimer1Count;
BYTE g_bDataBuffer[MAX_BUFFER_SIZE];

BYTE idata g_bRecordMuteGpio;

BYTE code *g_pbConfigDscr;
BYTE code *g_pHidDscr;
BYTE code *g_pHidReportDscr;
BYTE code *g_pProductStringDscr;

volatile BYTE xdata *g_pbXdataStart;

WORD g_wTemp, g_wTempA;
WORD g_wGpioIntMask, g_wGpioDirMask;
WORD g_wOldGpiData;

volatile WORD g_wGpiData;

AUDIO_CONTROL_STRUCT code	*g_pAudioControl;
CONVERTIBLE_CONTROL_TABLE_STRUCT code *g_pControlTable;

extern BYTE g_bCurrentSelector, g_bCurrentMute, g_bAttribute;
extern BYTE i_timercount;
extern BYTE idata g_bHidInRegsInfo;
extern BYTE idata g_bInputReport[MAX_HID_REPORT_SIZE];
extern BYTE idata g_bCurrentVolume[MAX_FEATURE_VOL_NUM][2];
extern BYTE idata g_bMinVolume[MAX_FEATURE_VOL_NUM];
extern BYTE idata g_bAlternateSetting[4];

extern CONVERTIBLE_CONTROL_TABLE_STRUCT code g_OriginControlTable1;
extern USB_CONTROL_STATE g_UsbCtrlState;

extern BYTE code T1_ConfigDscr, T1_HidDscr, Tx_HidReportDscr, ProductStringDscr;
extern BYTE code g_bFeatureVolumeTbl[MAX_FEATURE_VOL_NUM * 2];

/*********************************************************************
* Global/External function definition
*********************************************************************/
extern void PeriIntInit();
extern void SubmitUsbIntTransfer();
extern void HandleUsbCtrlTransfer();
extern void HandleUsbIntTransfer();

/*********************************************************************
* code area
*********************************************************************/
void Timer1Config()
{
    // based on MCU CLK=12MHz, the counter should be 10000 for 10ms. 
    g_wTemp = (0 - 10000);
    TH1 = MSB(g_wTemp);
    TL1 = LSB(g_wTemp);   

    TR1 = 1;
}

/** */
void InputReportDataReady()
{
	g_bInputReport[0] = HID_REPORT_ID_GENEGRIC;
		
	for(g_bIndex = 6; g_bIndex < MAX_HID_REPORT_SIZE; ++g_bIndex)
		g_bInputReport[g_bIndex] = 0;

	g_pbXdataStart = (BYTE xdata *)((WORD)((CTRL_REG_H_ADDR << 8) | CTRL_REG_L_ADDR));
	g_bInputReport[3] = CTRL_REG_H_ADDR;
	g_bInputReport[4] = CTRL_REG_L_ADDR;				
	for(g_bIndex = 0; g_bIndex < MIN(g_bHidInRegsInfo, (MAX_HID_REPORT_SIZE - 6)); ++g_bIndex)
	{
		g_bInputReport[6 + g_bIndex] = g_pbXdataStart[g_bIndex];
	}
}

/** */
void HandlePeriHidReport(BYTE peri_type)
{
	InputReportDataReady();
	g_bInputReport[5] |= peri_type;
	SubmitUsbIntTransfer();				 
	g_bInputReport[5] &= ~peri_type;
}

/** */
void PeriClkReset()
{
	// play and record clk ungated
	MOD_CLK_CTRL[0] &= ~(bmBIT5 | bmBIT4);
	
	if(g_bmSpdifOutEn == FALSE)
		MOD_CLK_CTRL[0] |= bmBIT3;

	// SPDIF_IN/IR clk gated
	MOD_CLK_CTRL[0] |= (bmBIT2 | bmBIT1);
	// I2C_slave adn ext. SPI clk gated
	MOD_CLK_CTRL[1] |= (bmBIT2 | bmBIT0);
}

/** */
void OriginInitialize()
{
	g_wGpioDirMask = 0xFD8F;		// 1 is for input
	g_wGpioIntMask = 0xFD8F;		// for GPI interrupt enable
	GPIO_Init(g_wGpioDirMask, g_wGpioIntMask, g_bmRemoteWakeupEn);
	g_wGpioIntMask &= g_wGpioDirMask;
	g_bRecordMuteGpio = DEFAULT_GPIO_RECORD_MUTE;
	
#ifndef __NUCLEODF__
	LED_Config(4, 5, 6);
	LED1_Switch(TRUE);
#endif

#ifdef _UART_
	UartBaudRateInit();
#endif
	
	g_bTimer1Count = 0;
	Timer1Config();
	
	PeriIntInit();

	//clk gated/un-gated for power saving
	PeriClkReset();
}

/** */
void OriginReset()
{
}

/** */
void OriginConfig()
{
	PDSW_Control(TRUE);
}

/** */
void OriginEvent()
{
}

/** */
void OriginGpi()
{
	g_wTempA = (g_wGpiData ^ g_wOldGpiData) & g_wGpioIntMask;
	for(g_bIndex2 = 0; g_bIndex2 < 16; g_bIndex2++)
	{			
		g_wTemp = ((WORD)bmBIT0) << g_bIndex2;
		if(g_wTempA & g_wTemp)
		{
			if(g_wGpiData & g_wTemp)
			{	
				g_wOldGpiData |= g_wTemp;	
								
				if(g_bIndex2 < 8)
				{				
					g_bInputReport[1] &= ~(LSB(g_wTemp));	
				}
				else
				{
					g_bInputReport[2] &= ~(MSB(g_wTemp));	
				}
			}
			else
			{
				g_wOldGpiData &= ~g_wTemp;	
											
				if(g_bIndex2 < 8)
				{				
					g_bInputReport[1] |= (LSB(g_wTemp));	
				}
				else
				{
					g_bInputReport[2] |= (MSB(g_wTemp));	
				}						
#ifndef __NUCLEODF__
				if(g_bIndex2 == g_bRecordMuteGpio)
				{
					g_pAudioControl->recordMute();
				}	
#endif																	
			}	
			
			// issue a HID report to the host
			HandlePeriHidReport(HID_INPUT_REPORT_INT_GPIO);

			break;				
		}
	}
	
	if(g_bIndex2 == 16)
	{
		g_bmGpiRequest = FALSE;			
	}		
}

/** */
void OriginIr()
{
	HandlePeriHidReport(HID_INPUT_REPORT_INT_IR);
}

/** */
void AgcReset()
{
	// AGC max. threshold
#ifdef __NUCLEODF__
	/**  -3db ==> 0.708     * 7FFFFF = 5A9FBD */
	/**  -6db ==> 0.5118... * 7FFFFF = 4026E6 */
	/**  -9db ==> 0.3548... * 7FFFFF = 2D6A86 */
	/** -13db ==> 0.2238... * 7FFFFF = 1CA7D7 */
	AGC_TH_CTRL[0] = 0xBD;
	AGC_TH_CTRL[1] = 0x9F;
	AGC_TH_CTRL[2] = 0x5A;
#else
	/** -3db ==> 0.708 * 7FFFFF = 5A9FBD */
	AGC_TH_CTRL[0] = 0xBD;
	AGC_TH_CTRL[1] = 0x9F;
	AGC_TH_CTRL[2] = 0x5A;
#endif	
		//analog path
	VR_READ_AGC_CTRL |= bmBIT7;
		
	AGC_MAX_GAIN = 9 + ((~(A_VOL(0x2D, g_bFeatureVolumeTbl[FEATURE_ADC_MIC * 2 + 0]))) & 0x3F);
	AGC_MIN_GAIN = (~(A_VOL(0x2D, g_bFeatureVolumeTbl[FEATURE_ADC_MIC * 2 + 1]))) & 0x3F;
	
	g_bmCurrentAgc = TRUE; 		// AGC. Si inicializo a FALSE ???
	g_pAudioControl->featureAgc();
	AGC_SIM_CTRL |= bmBIT4;		//will enable REC clipping led.
}

/** */
void variablesInit()
{
	g_bmRemoteWakeupEn = FALSE;
	g_bmSelfPower = FALSE;
	g_bmAdcHPSEn = TRUE;
	g_bmSpdifOutEn = TRUE;
  	g_bmAgcEn = TRUE;
  	g_bmAaPathEn = TRUE;
  	
	g_bCurrentSelector = SELECTOR_MIC;
	//g_bCurrentSelector = SELECTOR_LINE_IN;

	g_pControlTable = &g_OriginControlTable1;
	g_pAudioControl = g_pControlTable->pAudioControl;
	
	g_pbConfigDscr = &T1_ConfigDscr;
	g_pHidDscr = &T1_HidDscr;
	g_pHidReportDscr = &Tx_HidReportDscr;

#ifdef __NUCLEODF__
	InitProductName();
#else
	g_pProductStringDscr = &ProductStringDscr;
#endif
}

/** */
void PowerOnReset()
{
	// start the watchdog timer and refresh it.
	RefreshWdt();

	g_bExtIntEnable = (EX_INT1_EN | EX_TIMER0_EN | EX_INT0_EN);
	
	POR_Init();
	
	variablesInit();
	
	g_pControlTable->initialize();
	
	g_bmUsbPuReset = FALSE;
	g_bmUsbResume = FALSE;
	i_timercount = 0;
	EA = 1;
	TR0 = 1;
}	

/** */
void AnaCodecPowerReset()
{
	g_bTemp = InternSpiReadByte(ANALOG_CODEC_CTRL_9);
	
	/*
		bit5:AA-path enable
		bit4:DAC enable
		bit2:ADC enable
	*/
	g_bTemp = (g_bTemp | bmBIT5 | bmBIT4 | bmBIT2);
	InternSpiWriteByte(ANALOG_CODEC_CTRL_9, g_bTemp);
}

/** */
void usbEpReset()
{
	UsbResetAllEp();
		
	UsbSelectEp(ENDPOINT_CTRL);
	USB_EP_CTRL_X = bmBIT7;
	
	UsbSelectEp(ENDPOINT_ISO_OUT);
	USB_EP_CTRL_X = bmBIT7;		

	UsbSelectEp(ENDPOINT_ISO_IN);
	USB_EP_CTRL_X = bmBIT7;

	UsbSelectEp(ENDPOINT_INT);
	USB_EP_CTRL_X = bmBIT7;
}

/** */
void CodecReset()
{
	if(g_bmAdcHPSEn == FALSE)
	{
		InternSpiWriteByte(DIGITAL_CODEC_CTRL_0, 
				InternSpiReadByte(DIGITAL_CODEC_CTRL_0) & ~bmBIT6);			
	}

	// CLAMP=0, QA=0
	InternSpiWriteByte(ANALOG_CODEC_CTRL_D, 
		InternSpiReadByte(ANALOG_CODEC_CTRL_D) & ~(bmBIT4 | bmBIT2));

	// set IREF_TEST
	InternSpiWriteByte(ANALOG_CODEC_CTRL_10, 0x06);
	
	AdcCodecReset(FALSE, FALSE, FALSE, FALSE);
	DacCodecReset(FALSE, FALSE, FALSE, FALSE);

	// set mic as a recording source
	InternSpiWriteByte(ANALOG_CODEC_CTRL_A, 
		(InternSpiReadByte(ANALOG_CODEC_CTRL_A) |bmBIT7) & ~bmBIT6);	
}	

/** */
void volumeReset()
{
	MemZero((BYTE *)g_bCurrentVolume, MAX_FEATURE_VOL_NUM * 2);

	/** AGL. Volumenes por Defecto tras RESET */	
#ifdef __NUCLEODF__
	g_bCurrentVolume[FEATURE_DAC][0] = 0xF6;				// -10dB
	g_bCurrentVolume[FEATURE_DAC][1] = 0xF6;
	
	g_bCurrentVolume[FEATURE_ADC_MIC][0] = 0x14;			// 20dB. No tocar...
	g_bCurrentVolume[FEATURE_ADC_MIC][1] = 0x14;
	
	g_bCurrentVolume[FEATURE_MONITOR_MIC][0] = 0x00;		// 0dB
	g_bCurrentVolume[FEATURE_MONITOR_MIC][1] = 0x00;

	/** 20160316. Ganancias Personalizadas */
	setDefaultGains(_wgpi);
#else
	g_bCurrentVolume[FEATURE_DAC][0] = 0xF6;				// -10dB
	g_bCurrentVolume[FEATURE_DAC][1] = 0xF6;
	
	g_bCurrentVolume[FEATURE_ADC_MIC][0] = 0x14;			// 20dB
	g_bCurrentVolume[FEATURE_ADC_MIC][1] = 0x14;
	
	g_bCurrentVolume[FEATURE_MONITOR_MIC][0] = 0x00;		// 0dB
	g_bCurrentVolume[FEATURE_MONITOR_MIC][1] = 0x00;
#endif

	g_bMinVolume[FEATURE_DAC] = g_bFeatureVolumeTbl[(2 * FEATURE_DAC) + 1];
	g_bMinVolume[FEATURE_ADC_MIC] = g_bFeatureVolumeTbl[(2 * FEATURE_ADC_MIC) + 1];
	g_bMinVolume[FEATURE_MONITOR_MIC] = g_bFeatureVolumeTbl[(2 * FEATURE_MONITOR_MIC) + 1];
			
//	g_bCurrentMute = (BYTE)((bmBIT0 << FEATURE_MONITOR_LINE) | (bmBIT0 << FEATURE_MONITOR_MIC));
	g_bCurrentMute = (BYTE)(bmBIT0 << FEATURE_MONITOR_MIC);

	// update playback gain/mute registers
	g_bAttribute = FEATURE_DAC;
	g_pAudioControl->featureMute();
	g_pAudioControl->featureVolume();
	g_bAttribute = bmBIT4 | FEATURE_DAC;
	g_pAudioControl->featureVolume();
	
	// update record gain/mute registers
	g_bAttribute = FEATURE_ADC_MIC;
	g_pAudioControl->featureMute();
	g_pAudioControl->featureVolume();
	g_bAttribute = bmBIT4 | FEATURE_ADC_MIC;
	g_pAudioControl->featureVolume();
	
	// update MIC monitor gain/mute registers
	g_bAttribute = FEATURE_MONITOR_MIC;
	g_pAudioControl->featureMute();
	g_pAudioControl->featureVolume();
	g_bAttribute = bmBIT4 | FEATURE_MONITOR_MIC;
	g_pAudioControl->featureVolume();	
}

/** */
void HandleUsbReset()
{
	// disable timer0 when usb_reset after usb_suspend.
	if(g_bmUsbResume == FALSE)
	{
		TR0 = 0;
		AnaCodecPowerReset();
	}
	
	usbEpReset();
	CodecReset();

	if(g_bmSpdifOutEn)
	{
		SPDIF_FNUC_CTRL[1] |= bmBIT0;
		SPDIF_FNUC_CTRL[1] &= ~bmBIT1;
	}
	else
	{
		SPDIF_FNUC_CTRL[1] &= ~bmBIT0;
		SPDIF_FNUC_CTRL[1] |= bmBIT1;
	}
	
	MemZero(g_bInputReport, MAX_HID_REPORT_SIZE);
				
	g_pControlTable->reset();	

	g_bmGpiRequest = FALSE;

	g_wOldGpiData = 0xFFFF;
	
	g_UsbCtrlState = NONE_STAGE;
	g_bConfiguration = 0;

	MemZero(g_bAlternateSetting, 4);
	
	volumeReset();

	if(g_bmAgcEn)
		AgcReset();

	REC_SRC_TEST_SETTING &= ~bmBIT2;	//bypass soft_mute
}

/** */
void HandleUsbSuspend()
{
	if(UsbGetConfig() == 0)
		return;
	
	if(g_bmUsbResume)
		return;
	
	// EN_VAG, VMASTM
	InternSpiWriteByte(ANALOG_CODEC_CTRL_9, 
		(InternSpiReadByte(ANALOG_CODEC_CTRL_9) & ~bmBIT0) | bmBIT3);
	// VMICM, VLNIM
	InternSpiWriteByte(ANALOG_CODEC_CTRL_1, 
		InternSpiReadByte(ANALOG_CODEC_CTRL_1) | (bmBIT3 | bmBIT2));

	// GPIs need to be connected to pull-down resistances for power-saving when in suspend mode.
	GpioPuPdSetting(g_wGpioDirMask, 0);
	
	// start timer0 to count down 3s for entering suspend status
	i_timercount = 0;
	TH0 = 0;
	TL0 = 0; 
	TR0 = 1;
}

/** */
void HandleUsbResume()
{
	if(g_bmUsbResume)
		return; 

	// disable timer0 to avoid entering usb_suspend
	TR0 = 0;
	
	AnaCodecPowerReset();
	
	// set EN_XLOCOM high
	InternSpiWriteByte(ANALOG_CODEC_CTRL_A, 
		InternSpiReadByte(ANALOG_CODEC_CTRL_A) | bmBIT1);
	// VMASTM, EN_VAG, EN_VBG
	InternSpiWriteByte(ANALOG_CODEC_CTRL_9, 
		(InternSpiReadByte(ANALOG_CODEC_CTRL_9) & ~bmBIT3) | (bmBIT1 | bmBIT0));

	GpioPuPdSetting(g_wGpioDirMask, 1);

	// start timer0 to end up pop-noise procedure 
	i_timercount = 0;
	g_bmUsbResume = TRUE;
	TH0 = 0;
	TL0 = 0;
	TR0 = 1;

	AdcCodecReset(FALSE, FALSE, FALSE, FALSE);
	DacCodecReset(FALSE, FALSE, FALSE, FALSE);
}

#ifdef __NUCLEODF__
	/**
	 * AGL...
	 */
	void InitProductName()
	{
		// Leer el estado GPI....
		_wgpi = (((WORD)(GPI_DATA[1])) << 8) | GPI_DATA[0];
		_wgpi = _wgpi &  0x040C;
	
		if (_wgpi == 0)
			g_pProductStringDscr = &ProductStringDscr_01;
		else if (_wgpi== 0x0400)
			g_pProductStringDscr = &ProductStringDscr_02;
		else if (_wgpi == 0x0008)
			g_pProductStringDscr = &ProductStringDscr_03;
		else if (_wgpi == 0x0408)
			g_pProductStringDscr = &ProductStringDscr_04;
		else
			g_pProductStringDscr = &ProductStringDscr;
	}
	
	/**
	 * AGL...
	 */
	void leds(BYTE nled, BOOL val)
	{
		BYTE _bgp0 = GPO_DATA[0];		// GPIO-0 ... GPIO-7 
	
		if (val == TRUE)
			_bgp0 |= nled;
		else
			_bgp0 &= ~nled;
	
		GPO_DATA[0] = _bgp0;
	}
	static BOOL _tick = TRUE;
	void tickled()
	{
		/** */
		
		if ((g_bTimer1Count % 100)==0)
		{
			leds(LED1, _tick);
			_tick = !_tick;
		}
	}

	/**
		AGL. 20151026	
	 */
	void manual_reset(BOOL val)
	{
		BYTE _bgp0 = GPO_DATA[0];		// GPIO-0 ... GPIO-7 
	
		if (val == FALSE)		  		// Logica invertida en GPIO-6
			_bgp0 |= 0x40;
		else
			_bgp0 &= ~(0x40);
	
		GPO_DATA[0] = _bgp0;
	}

	/**
		AGL. 20160316. Se configuran valores por defecto segun la placa
	*/
	void setDefaultGains(int slave)
	{
		switch (slave)
		{
			case 0x0000:											// ESC#1. MIC +6dB CAS -19dB
			case 0x0400:											// ESC#2. MIC +6dB CAS -19dB

				g_bCurrentVolume[FEATURE_DAC][0] = 0xED;				// RIGHT 	Casco
				g_bCurrentVolume[FEATURE_DAC][1] = 0xED;				// LEFT 	GRB...
	
				g_bCurrentVolume[FEATURE_ADC_MIC][0] = 0x06;			// RIGHT	Microfono
				g_bCurrentVolume[FEATURE_ADC_MIC][1] = 0x06;			// LEFT		Retorno...
	
				g_bCurrentVolume[FEATURE_MONITOR_MIC][0] = 0x00;		// 
				g_bCurrentVolume[FEATURE_MONITOR_MIC][1] = 0x00;
				break;

			case 0x0008:											// ESC#3. RET +6 B ALT -6
			case 0x0408:											// ESC#4. RET +6 B ALT -6

				g_bCurrentVolume[FEATURE_DAC][0] = 0xFA;				// RIGHT	Altavoz VHF / Altavoz LC
				g_bCurrentVolume[FEATURE_DAC][1] = 0xFA;				// LEFT		Altavoz HF  / Salida GRB..
	
				g_bCurrentVolume[FEATURE_ADC_MIC][0] = 0x06;			// RIGHT	Retorno 1
				g_bCurrentVolume[FEATURE_ADC_MIC][1] = 0x06;			// LEFT		Retorno 2
	
				g_bCurrentVolume[FEATURE_MONITOR_MIC][0] = 0x00;		// 0dB
				g_bCurrentVolume[FEATURE_MONITOR_MIC][1] = 0x00;
				break;
		}
	}

#endif

/** */
void main()
{
	SP -=4;

	PowerOnReset();

#ifdef __NUCLEODF__

	leds(LED0, TRUE);
	manual_reset(FALSE);

#endif

	HubInSuspend();

	// Main Loop
	while(TRUE)
	{
		if(g_bmUsbPuReset == TRUE)
		{
			if(UsbEventRst())
			{
				UsbClrEventRst();
				AntiPopProcedure();
				HandleUsbReset();
			}
			
			if(UsbEventSuspend())
			{
				HandleUsbSuspend();	
				UsbClrEventSuspend();
			}
    	
			if(UsbEventResume())
			{
				HandleUsbResume();
				UsbClrEventResume();
			}
					
			if(UsbEventEpCtrl())
			{		
				UsbClrEventEpCtrl();	
				HandleUsbCtrlTransfer();	
			}
					
			if(UsbEventEpInt())
			{
				HandleUsbIntTransfer();
				UsbClrEventEpInt();
			}
    	
			if(g_bmGpiRequest == TRUE)
			{	
				g_pControlTable->gpi();	
			}
    	
			g_pControlTable->event();
		}

		RefreshWdt();
	}
}	
