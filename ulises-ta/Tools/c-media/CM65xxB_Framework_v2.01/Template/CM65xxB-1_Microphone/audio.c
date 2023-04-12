#include <intrins.h>
#include "types.h"
#include "registers.h"
#include "config.h"
#include "cm65xx.h"
#include "audio.h"
#include "usb.h"

/*********************************************************************
* External library variable declaration
*********************************************************************/
extern BYTE code g_bSamplingRateTbl[SAMPLING_RATE_NUM][5];

/*********************************************************************
* External library function
*********************************************************************/
extern void PlaybackMuteControl(BOOL analog, BOOL mute);
extern void LED2_Switch(BOOL open);
extern void SPDIFOutMuteSwitch(BOOL mute);
extern void RecordMuteControl(BOOL analog, BOOL mute);
extern void LED3_Switch(BOOL open);
extern void MicInAaMuteControl(BOOL mute);
extern void LineInAaMuteControl(BOOL mute);
extern void SPDIFInMuteSwitch(BOOL mute);
extern void PlaybackMinVolHandle(BOOL analog, BYTE ch);
extern void PlaybackVolControl(BOOL analog, BYTE ch, BYTE vol);
extern void RecordVolControl(BOOL analog, BOOL mic, BYTE ch, BYTE vol);
extern void AApathVolControl(BOOL mic, BYTE ch, BYTE vol);
extern void RecordfromSPDIF(BOOL en);
extern void SelectRecPath(BYTE source);
extern void SetPlaySampleRate(BYTE sr);
extern void SetPlayBitResolution(BOOL bit16);
extern void SetRecordSampleRate(BYTE sr);
extern void SetRecordBitResolution(BOOL bit16);
extern void RecordMonoEnable(BOOL mono);
extern void C2MCopy(BYTE *dest, BYTE code *src, BYTE size);

/*********************************************************************
* Global/External variable definition
*********************************************************************/
BYTE g_bCurrentSelector, g_bCurrentMute, g_bAttribute;

BYTE idata g_bCurrentVolume[MAX_FEATURE_VOL_NUM][2];
BYTE idata g_bMinVolume[MAX_FEATURE_VOL_NUM];
BYTE idata g_bCurrentSampleRate[2][3];

extern BOOL g_bmSpdifOutEn;

extern volatile BOOL g_bmUsbResume;

extern BYTE g_bIndex;
extern BYTE g_bTemp, g_bTempA, g_bTempB;
extern BYTE *g_pbDataBuffer;
extern BYTE g_bDataBuffer[MAX_BUFFER_SIZE];

extern BYTE idata g_bAlternateSetting[4];

extern WORD g_wDataOffset, g_wDataLength;

extern AUDIO_CONTROL_STRUCT code *g_pAudioControl;
extern USB_CONTROL_COMMAND_STRUCT g_UsbCtrlCmnd;

/*********************************************************************
* Global/External function definition
*********************************************************************/
void OriginFeatureVolume();
void OriginFeatureMute();
void OriginFeatureAgc();
void OriginSetSelector();
void OriginRecordMute();
void OriginSetFrequency1();
void OriginSetFrequency2();

extern void OriginInitialize();
extern void OriginReset();
extern void OriginConfig();
extern void OriginEvent();
extern void OriginGpi();
extern void OriginIr();

/*********************************************************************
* code area
*********************************************************************/
BYTE code g_bFeatureVolumeTbl[MAX_FEATURE_VOL_NUM * 2] = 
{
	0x00, 0xC2,		// for Play			Max:0	Min:-62
	0x0C, 0xE6,		// for Line			Max:12	Min:-26
	0x1E, 0x00,		// for Mic			Max:30	Min:0
	0x1E, 0x00,		// for Stero Mixer	Max:30	Min:0
	0x0C, 0xE2,		// for AA-Line		Max:12	Min:-30
	0x16, 0xF1,		// for AA-Mic		Max:22	Min:-15
};

// HeadSet
#define T1_MAX_UNIT_NUM		4
BYTE code OriginAcUnitTable1[T1_MAX_UNIT_NUM + 1] =
{
	AC_NODEF,
	AC_IT, 			
	AC_OT, 
	(FEATURE_ADC_MIC << 4) | AC_FEATURE,
	((1 << 4) | AC_SELECTOR)
};	

AUDIO_CONTROL_STRUCT code OriginAudioControl1 =
{
	OriginFeatureVolume,
	OriginFeatureMute,
	OriginFeatureAgc,
	OriginSetSelector,
	OriginRecordMute,
	OriginAcUnitTable1,
	{{0x11, 0x82, OriginSetFrequency2}, 
	 {bmBIT7, 0xFF, NULL}}
};

DEVICE_CONTROL_STRUCT code OriginDeviceControl1 =
{
	1, 2
};	

CONVERTIBLE_CONTROL_TABLE_STRUCT code g_OriginControlTable1 =
{
	&OriginDeviceControl1,
	&OriginAudioControl1,	
	OriginInitialize,
	OriginReset,
	OriginConfig,	
	OriginEvent,
	OriginGpi,
	OriginIr
};

void freq2CtrlByte()
{
	for(g_bIndex = 0; g_bIndex < SAMPLING_RATE_NUM; ++g_bIndex)
	{
		if((g_pbDataBuffer[1] == g_bSamplingRateTbl[g_bIndex][1]) &&
		   	(g_pbDataBuffer[2] == g_bSamplingRateTbl[g_bIndex][0]))
			break;
	}

	if(g_bTempA)
	{
		// handle spdif out sampling rate
		if(g_bIndex == SAMPLING_RATE_NUM)	
		{		
			g_bTemp = CTRL_SPDIF_OUT_SAMPLING_RATE_NONE;
		}
		else
		{
			g_bTemp = g_bSamplingRateTbl[g_bIndex][4];
		}	
	}
	else	
	{
		if(SAMPLING_RATE_NUM == g_bIndex)
		{
			// If not found in the table, set 48KHz as default.
			g_bIndex = 6;
		}
	
		g_bTemp = g_bSamplingRateTbl[g_bIndex][3];
	}	
}

void StartSpdifOut()
{
	g_bTempA = 1;
	freq2CtrlByte();
	
	if(g_bTemp == CTRL_SPDIF_OUT_SAMPLING_RATE_NONE)
	{
		SPDIFOutMuteSwitch(1);
	}
	else
	{
		SPDIFOutMuteSwitch(0);
	}
}	

void OriginSetFrequency1()
{		
	g_bTempA = 0;
	freq2CtrlByte();	// g_bTemp is assigned as a new frequency.

	SetPlaySampleRate(g_bTemp);
	
	g_bIndex = (HI_NIBBLE(g_bAttribute)) & (bmBIT1 | bmBIT0);
	if(g_bAlternateSetting[g_bIndex] == 2)
	{
		SetPlayBitResolution(0);
	}
	else
	{
		SetPlayBitResolution(1);
	}

	if(g_bmSpdifOutEn)
	{
		StartSpdifOut();
	}
}

void OriginSetFrequency2()
{
	g_bTempA = 0;
	freq2CtrlByte(); // g_bTemp is assigned as a new freqency.

	SetRecordSampleRate(g_bTemp);
	
	g_bIndex = (HI_NIBBLE(g_bAttribute)) & (bmBIT1 | bmBIT0);
	if(g_bAlternateSetting[g_bIndex] == 2)
	{
		SetRecordBitResolution(0);
	}
	else
	{
		SetRecordBitResolution(1);
	}

	// check if ADC is mono or not.
	RecordMonoEnable(g_bAttribute & bmBIT6);
}	

void OriginFeatureMute()
{
 	switch(g_bAttribute)
 	{
 		case FEATURE_DAC:
 			if(g_bCurrentMute & (bmBIT0 << FEATURE_DAC))
 			{
				PlaybackMuteControl(1, 1);
 				
 				LED2_Switch(1);

				if(g_bmSpdifOutEn)
				{
					SPDIFOutMuteSwitch(1);
				}
 			}
 			else
 			{ 				
 				PlaybackMuteControl(1, 0);
 				
 				LED2_Switch(0);

				if(g_bmSpdifOutEn)
				{
					SPDIFOutMuteSwitch(0);
				}
 			}		
			break;			
 		
 		case FEATURE_ADC_MIC:
 		case FEATURE_ADC_LINE:
		case FEATURE_MIXER:
			if(g_bCurrentSelector == SELECTOR_SPDIF_IN)
			{
				break;
			}
			else
			{
				switch(g_bCurrentSelector)
				{
					case SELECTOR_MIC:
						g_bTemp = FEATURE_ADC_MIC;
						break;

					case SELECTOR_LINE_IN:
						g_bTemp = FEATURE_ADC_LINE;
						break;

					default:
						g_bTemp = FEATURE_MIXER;
						break;
				}
			}
 
 			if(g_bCurrentMute & (bmBIT0 << g_bTemp))
			{
				RecordMuteControl(1, 1);
				
				LED3_Switch(1);								
			}
			else
			{
				RecordMuteControl(1, 0);
				
				LED3_Switch(0);
			}		 					
			break;

		case FEATURE_MONITOR_MIC:
			if(g_bCurrentMute & (bmBIT0 << FEATURE_MONITOR_MIC))
			{
				MicInAaMuteControl(1);
			}
			else
			{
				MicInAaMuteControl(0);
			}
			break;
		
		case FEATURE_MONITOR_LINE:
			if(g_bCurrentMute & (bmBIT0 << FEATURE_MONITOR_LINE))
			{
				LineInAaMuteControl(1);
			}
			else
			{
				LineInAaMuteControl(0);
			}
			break;
 		
		case FEATURE_SPDIF:
			if(g_bCurrentSelector == SELECTOR_SPDIF_IN)
			{
				if(g_bCurrentMute & (bmBIT0 << FEATURE_SPDIF))
				{
					SPDIFInMuteSwitch(1);
				}
				else
				{
					SPDIFInMuteSwitch(0);
				}
			}
			break;
			
		default:
			break;
 	}		
}	

void OriginFeatureVolume()
{
	g_bTempB = HI_NIBBLE(g_bAttribute);
	
 	switch(LO_NIBBLE(g_bAttribute))
 	{
 		case FEATURE_DAC: 		
 			if(((SBYTE)g_bCurrentVolume[FEATURE_DAC][g_bTempB]) <= ((SBYTE)g_bMinVolume[FEATURE_DAC]))
 			{
 				g_bCurrentVolume[FEATURE_DAC][g_bTempB] = g_bMinVolume[FEATURE_DAC];
 				
 				PlaybackMinVolHandle(1, g_bTempB);
 			}
			else
			{	
				PlaybackVolControl(1, g_bTempB, g_bCurrentVolume[FEATURE_DAC][g_bTempB]);
			}
			break;
		
		case FEATURE_ADC_MIC:
		case FEATURE_ADC_LINE:
		case FEATURE_MIXER:
			if(g_bCurrentSelector == SELECTOR_SPDIF_IN)
			{
				break;
			}
			else
			{
				switch(g_bCurrentSelector)
				{
					case SELECTOR_MIC:
						g_bTemp = FEATURE_ADC_MIC;
						break;

					case SELECTOR_LINE_IN:
						g_bTemp = FEATURE_ADC_LINE;
						break;

					default:
						g_bTemp = FEATURE_MIXER;
						break;
				}
			}
 			
 			if(((SBYTE)g_bCurrentVolume[g_bTemp][g_bTempB]) < ((SBYTE)g_bMinVolume[g_bTemp]))
 			{
 				g_bCurrentVolume[g_bTemp][g_bTempB] = g_bMinVolume[g_bTemp];
 			}	
 				
			if((g_bTemp == FEATURE_ADC_MIC) || (g_bTemp == FEATURE_MIXER))
			{
					RecordVolControl(1, 1, g_bTempB, g_bCurrentVolume[g_bTemp][g_bTempB]);
			}
			else
			{
					RecordVolControl(1, 0, g_bTempB, g_bCurrentVolume[g_bTemp][g_bTempB]);
			}
			break;	
 						
		case FEATURE_MONITOR_MIC:
			if(((SBYTE)g_bCurrentVolume[FEATURE_MONITOR_MIC][g_bTempB]) < ((SBYTE)g_bMinVolume[FEATURE_MONITOR_MIC]))
 			{
 				g_bCurrentVolume[FEATURE_MONITOR_MIC][g_bTempB] = g_bMinVolume[FEATURE_MONITOR_MIC];
 			}
			
			AApathVolControl(1, g_bTempB, g_bCurrentVolume[FEATURE_MONITOR_MIC][g_bTempB]);
			break;
		  
		case FEATURE_MONITOR_LINE:
			if(((SBYTE)g_bCurrentVolume[FEATURE_MONITOR_LINE][g_bTempB]) < ((SBYTE)g_bMinVolume[FEATURE_MONITOR_LINE]))
 			{
 				g_bCurrentVolume[FEATURE_MONITOR_LINE][g_bTempB] = g_bMinVolume[FEATURE_MONITOR_LINE];
 			}
			
			AApathVolControl(0, g_bTempB, g_bCurrentVolume[FEATURE_MONITOR_MIC][g_bTempB]);
			break;
	      			  		 		
 		default:
			break;
 	}
}	

void OriginFeatureAgc()
{			
}

void OriginSetSelector()
{
	switch(g_bCurrentSelector)
	{
		case SELECTOR_MIC:
			// source switch
			SelectRecPath(SELECTOR_MIC);
			// update volume
			RecordVolControl(1, 1, 0, g_bCurrentVolume[FEATURE_ADC_MIC][0]);
			RecordVolControl(1, 1, 1, g_bCurrentVolume[FEATURE_ADC_MIC][1]);
			// update mute status
			if(g_bCurrentMute & (bmBIT0 << FEATURE_ADC_MIC))
			{
				RecordMuteControl(1, 1);
			}
			else
			{
				RecordMuteControl(1, 0);
			}
			
			RecordfromSPDIF(0);
			break;
		
		case SELECTOR_LINE_IN:
			// source switch
			SelectRecPath(SELECTOR_LINE_IN);
			// update volume
			RecordVolControl(1, 0, 0, g_bCurrentVolume[FEATURE_ADC_LINE][0]);
			RecordVolControl(1, 0, 1, g_bCurrentVolume[FEATURE_ADC_LINE][1]);
			// update mute status
			if(g_bCurrentMute & (bmBIT0 << FEATURE_ADC_LINE))
			{
				RecordMuteControl(1, 1);
			}
			else
			{
				RecordMuteControl(1, 0);
			}

			RecordfromSPDIF(0);
			break;
		  
		case SELECTOR_SMIX_IN:
			// source switch
			SelectRecPath(SELECTOR_SMIX_IN);
			// update volume
			RecordVolControl(1, 1, 0, g_bCurrentVolume[FEATURE_MIXER][0]);
			RecordVolControl(1, 1, 1, g_bCurrentVolume[FEATURE_MIXER][1]);
			// update mute status
			if(g_bCurrentMute & (bmBIT0 << FEATURE_MIXER))
			{
				RecordMuteControl(1, 1);
			}
			else
			{
				RecordMuteControl(1, 0);
			}
			
			RecordfromSPDIF(0);
			break;
		
		case SELECTOR_SPDIF_IN:
			// source switch
			RecordfromSPDIF(1);
			// update mute status
			if(g_bCurrentMute & (bmBIT0 << FEATURE_SPDIF))
			{
				SPDIFInMuteSwitch(1);
			}
			else
			{
				SPDIFInMuteSwitch(0);
			}
			break;
		
		default:
			break;  
	}	
}	

void OriginRecordMute()
{
	if(g_bCurrentSelector == SELECTOR_SPDIF_IN)
	{
		if(g_bCurrentMute & (bmBIT0 << FEATURE_SPDIF))
		{
			g_bCurrentMute &= ~(bmBIT0 << FEATURE_SPDIF);
			SPDIFInMuteSwitch(0);
			LED3_Switch(0);
		}
		else
		{
			g_bCurrentMute |= (bmBIT0 << FEATURE_SPDIF);
			SPDIFInMuteSwitch(1);
			LED3_Switch(1);
		}
	}
	else
	{
		switch(g_bCurrentSelector)
		{
			case SELECTOR_MIC:
				g_bTemp = (bmBIT0 << FEATURE_ADC_MIC);
				break;
		
			case SELECTOR_LINE_IN:
				g_bTemp = (bmBIT0 << FEATURE_ADC_LINE);
				break;	
		
			case SELECTOR_SMIX_IN:
				g_bTemp = (bmBIT0 << FEATURE_MIXER);
				break;
				
			default:
				 return;	
		}
	
		if(g_bCurrentMute & g_bTemp)
		{
			g_bCurrentMute &= ~g_bTemp;
		
			RecordMuteControl(1, 0);
		
			LED3_Switch(0);
		}
		else
		{
			g_bCurrentMute |= g_bTemp;
		
			RecordMuteControl(1, 1);
		
			LED3_Switch(1);				
		}
	}
}	

BOOL tackleSelector(BOOL dataStage)
{
	if(g_UsbCtrlCmnd.bmRequestType & bmBIT7)	// Device to Host
	{		
		switch(g_UsbCtrlCmnd.bRequest)
		{
			case CMD_GET_CURRENT:
				g_bDataBuffer[0] = g_bCurrentSelector;
			  break;
			  
			case CMD_GET_MIN:
				g_bDataBuffer[0] = 1;
			  break;	
				
			case CMD_GET_MAX:
				g_bDataBuffer[0] = g_bAttribute;
			  break;
			
			case CMD_GET_RES:
				g_bDataBuffer[0] = 1;
			  break;	
			
			default:
				return FALSE;	
		}	
		
		g_wDataLength = 1;
		g_pbDataBuffer = g_bDataBuffer;			
	}
	else
	{
		if(g_UsbCtrlCmnd.bRequest == CMD_SET_CURRENT)
		{
			if(dataStage == FALSE)
			{
				g_wDataLength = 1;
				g_pbDataBuffer = &g_bCurrentSelector;
			}
			else
			{
				g_pAudioControl->setSelector();	
			}
		}						
	}	
	
	return TRUE;
}

BOOL tackleFeatureVolume(BOOL dataStage)
{
	if(g_UsbCtrlCmnd.bmRequestType & bmBIT7)	// Device to Host
	{	
		switch(g_UsbCtrlCmnd.bRequest)
		{
			case CMD_GET_CURRENT:
				g_bTemp = LSB(g_UsbCtrlCmnd.wValue) - 1;				
				g_bDataBuffer[0] = 0x00;
				g_bDataBuffer[1] = g_bCurrentVolume[g_bAttribute][g_bTemp];
			  break;
			  
			case CMD_GET_MIN:							
			case CMD_GET_MAX:			
				C2MCopy(g_bDataBuffer, g_bFeatureVolumeTbl + (g_bAttribute * 2), 2);
				
				if(g_UsbCtrlCmnd.bRequest == CMD_GET_MAX)
				{
					g_bDataBuffer[1] = g_bDataBuffer[0];
				}
				g_bDataBuffer[0] = 0x00;
			  break;
				
			case CMD_GET_RES:
				g_bDataBuffer[0] = 0x00;	
				g_bDataBuffer[1] = 0x01;
			  break;
			
			default:
			  return FALSE;  	
		}
		
		g_wDataLength = 2;
		g_pbDataBuffer = g_bDataBuffer;
	}
	else
	{
		if(g_UsbCtrlCmnd.bRequest == CMD_SET_CURRENT)
		{
			if(dataStage == FALSE)
			{
				g_wDataLength = 2;
				g_pbDataBuffer = g_bDataBuffer;
			}
			else
			{	
				g_bTemp = LSB(g_UsbCtrlCmnd.wValue) - 1;				
				
				if(g_bDataBuffer[0] & bmBIT7)	// because of 1dB resolution, volume adjustment is needed.
				{
					g_bCurrentVolume[g_bAttribute][g_bTemp] = g_bDataBuffer[1] + 1;
				}
				else
				{
					g_bCurrentVolume[g_bAttribute][g_bTemp] = g_bDataBuffer[1];
				}
				
				if(g_bTemp)
				{
					g_bAttribute |= bmBIT4;
				}

				g_pAudioControl->featureVolume();							
			}
		}
		else
		{
			return FALSE;  				
		}
	}	

	return TRUE;
}	

BOOL tackleFeatureMute(BOOL dataStage)
{
	if(g_UsbCtrlCmnd.bmRequestType & bmBIT7)	// Device to Host
	{	
		if(g_UsbCtrlCmnd.bRequest == CMD_GET_CURRENT)
		{
			g_bDataBuffer[0] = (g_bCurrentMute >> g_bAttribute) & bmBIT0;
			g_wDataLength = 1;
			g_pbDataBuffer = g_bDataBuffer;		
		}
		else
		{
			return FALSE;  			
		}
	}
	else
	{
		if(g_UsbCtrlCmnd.bRequest == CMD_SET_CURRENT)
		{
			if(dataStage == FALSE)
			{
				g_wDataLength = 1;
				g_pbDataBuffer = g_bDataBuffer;				
			}
			else
			{
				if(g_bDataBuffer[0])
				{
					g_bCurrentMute |= (bmBIT0 << g_bAttribute);
				}
				else
				{
					g_bCurrentMute &= (~(bmBIT0 << g_bAttribute));
				}		

				// for anti-pop noise
				if((g_bAttribute == FEATURE_MONITOR_MIC) || (g_bAttribute == FEATURE_MONITOR_LINE))
				{
					if(g_bmUsbResume)
						return TRUE;
				}
				
				g_pAudioControl->featureMute();
			}
		}
		else
		{
			return FALSE;  			
		}
	}		
	
	return TRUE;	
}

BOOL TackleAudioControl(BOOL dataStage)
{
	g_bTemp = LO_NIBBLE(g_pAudioControl->pAcUnitTable[MSB(g_UsbCtrlCmnd.wIndex)]);
	g_bAttribute = HI_NIBBLE(g_pAudioControl->pAcUnitTable[MSB(g_UsbCtrlCmnd.wIndex)]);
		
	switch(g_bTemp)
	{
		case AC_FEATURE:
			switch(MSB(g_UsbCtrlCmnd.wValue))
			{
				case MUTE_CONTROL:
					return (tackleFeatureMute(dataStage));
					break;
					
				case VOLUME_CONTROL:
					return (tackleFeatureVolume(dataStage));
					break;
					
				case BASS_CONTROL:
				case TREBLE_CONTROL:
					break;
					
				case AUTOMATIC_GAIN_CONTROL:
					break;
					
				default:
					break;
			}
		  break;
		  
		case AC_SELECTOR:
			return (tackleSelector(dataStage));
			break;

		case AC_MIXER:
			break;
		
		case AC_IT:
			break;
	}		
	
	return FALSE;
}	

BOOL TackleAudioStream(BOOL dataStage)
{
	if(MSB(g_UsbCtrlCmnd.wValue) == SAMPLING_FREQ_CONTROL)
	{
		for(g_bIndex = 0; g_bIndex < 2; g_bIndex++)
		{
			if(LSB(g_UsbCtrlCmnd.wIndex) == g_pAudioControl->asControl[g_bIndex].endpoint)
				break;
		}
		if(g_bIndex == 2)
		{
			return FALSE;
		}

		if((LSB(g_UsbCtrlCmnd.wIndex)) & bmBIT7)
		{
			g_bTemp = 1;	
		}	
		else
		{
			g_bTemp = 0;
		}
				
		if(g_UsbCtrlCmnd.bmRequestType & bmBIT7)	// Device to Host
		{
			g_wDataLength = 3;
			switch(g_UsbCtrlCmnd.bRequest)
			{
				case CMD_GET_CURRENT:
					g_bDataBuffer[0] = g_bCurrentSampleRate[g_bTemp][0];
					g_bDataBuffer[1] = g_bCurrentSampleRate[g_bTemp][1];
					g_bDataBuffer[2] = g_bCurrentSampleRate[g_bTemp][2];
					break;
			  
				case CMD_GET_MIN:
					g_bDataBuffer[0] = g_bSamplingRateTbl[0][2];
					g_bDataBuffer[1] = g_bSamplingRateTbl[0][1];
					g_bDataBuffer[2] = g_bSamplingRateTbl[0][0];
					break;
				
				case CMD_GET_MAX:
					g_bDataBuffer[0] = g_bSamplingRateTbl[SAMPLING_RATE_NUM - 1][2];
					g_bDataBuffer[1] = g_bSamplingRateTbl[SAMPLING_RATE_NUM - 1][1];
					g_bDataBuffer[2] = g_bSamplingRateTbl[SAMPLING_RATE_NUM - 1][0];
					break;
				
				case CMD_GET_RES:
					g_bDataBuffer[0] = 0x05;
					g_bDataBuffer[1] = 0x00;
					g_bDataBuffer[2] = 0x00;
					break;
			
				default:
					return FALSE;
					break;
			}
			g_pbDataBuffer = g_bDataBuffer;
		}
		else
		{
			if(g_UsbCtrlCmnd.bRequest == CMD_SET_CURRENT)		
			{
				if(dataStage == FALSE)
				{
					g_wDataLength = 3;
					g_pbDataBuffer = g_bCurrentSampleRate[g_bTemp];
				}
				else	
				{
					g_bAttribute = g_pAudioControl->asControl[g_bIndex].attribute;
					g_pAudioControl->asControl[g_bIndex].setFrequency();					
				}
			}
			else
			{
				return FALSE;	
			}
		}	
	}	
		
	return TRUE;
}	

BOOL TackleSetAudioInterface()
{
	g_bIndex = (LSB(g_UsbCtrlCmnd.wIndex));
	
	g_bTemp = g_pAudioControl->asControl[g_bIndex - 1].attribute;	
	if(g_bTemp & bmBIT7)		// invalid
	{
		return FALSE; 
	}
	
	if((LSB(g_UsbCtrlCmnd.wValue)) > (LO_NIBBLE(g_bTemp)))
	{
		return FALSE; 
	}
	
	g_bTemp = ((g_pAudioControl->asControl[g_bIndex - 1].endpoint) & bmBIT7);
	if((LSB(g_UsbCtrlCmnd.wValue)) == 0)
	{
		if(g_bTemp == 0)
		{
			UsbSelectEp(ENDPOINT_ISO_OUT);
			UsbStopEp();
		}
		else
		{
			UsbSelectEp(ENDPOINT_ISO_IN);
			UsbStopEp();
		}
	}
	else
	{
		if(g_bTemp == 0)
		{
			UsbSelectEp(ENDPOINT_ISO_OUT);
			UsbStartEp();
		}
		else
		{
			UsbSelectEp(ENDPOINT_ISO_IN);
			UsbStartEp();
		}
	}

	UsbSelectEp(ENDPOINT_CTRL);
  	
	return TRUE;
}

