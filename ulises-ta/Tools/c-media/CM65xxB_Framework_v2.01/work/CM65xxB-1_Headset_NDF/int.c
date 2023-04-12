#include <intrins.h>
#include "types.h"
#include "registers.h"
#include "usb.h"
#include "audio.h"
#include "cm65xx.h"

/*********************************************************************
* External library variable declaration
*********************************************************************/
extern BOOL g_bmUsbPuReset;

/*********************************************************************
* Global/External variable definition
*********************************************************************/
BYTE i_bTemp, i_timercount;

extern volatile BOOL g_bmGpiRequest;
extern volatile BOOL g_bmUsbResume;

extern BYTE g_bCurrentMute, g_bTimer1Count;

extern volatile WORD g_wGpiData;
extern void tickled();

/*********************************************************************
* Global/External function definition
*********************************************************************/
extern void Timer1Config();

/**
 *******************************************************************
 * code area
 ********************************************************************
 */
static BYTE intSpiRead(BYTE reg)
{
	IN_SPI_DATA[0] = bmBIT7 | reg;
	IN_SPI_CTRL |= (bmBIT7 | 0x06);
	while(IN_SPI_CTRL & bmBIT7)
		;
	return IN_SPI_DATA[1];
}

/**
 *
 */
static void intSpiWrite(BYTE reg, BYTE temp)
{
	IN_SPI_DATA[0] = reg;
	IN_SPI_DATA[1] = temp;
	IN_SPI_CTRL |= (bmBIT7 | 0x06);
	while(IN_SPI_CTRL & bmBIT7)
		;
}

/**
 *
 */
void HandleInt0Isr() using 1
{
}

/**
 *
 */
void HandleTimer0Isr() using 1
{
	++i_timercount;
	
	if((i_timercount == (2 * pop_noise_1s)) && (g_bmUsbResume == TRUE))
	{
		TR0 = 0;
		g_bmUsbResume = FALSE;

		// upadte VMICM/VLNIM mute status
		i_bTemp = intSpiRead(ANALOG_CODEC_CTRL_1);		
		i_bTemp = (g_bCurrentMute & (BYTE)(bmBIT0 << FEATURE_MONITOR_MIC)) ? (i_bTemp | bmBIT3) : (i_bTemp & ~bmBIT3);
		i_bTemp = (g_bCurrentMute & (BYTE)(bmBIT0 << FEATURE_MONITOR_LINE)) ? (i_bTemp | bmBIT2) : (i_bTemp & ~bmBIT2);
		intSpiWrite(ANALOG_CODEC_CTRL_1, i_bTemp);
 	}
	else if((i_timercount == (3 * pop_noise_1s)) && (g_bmUsbPuReset == FALSE))
	{
		TR0 = 0;
		g_bmUsbPuReset = TRUE; 
		
		UsbEnableUsb();
		USB_PHY_POW &= ~bmBIT1;
	}
	else if((i_timercount == (3 * pop_noise_1s)) && (g_bmUsbPuReset == TRUE))
	{
		TR0 = 0; 
		
		// EN_VBG, EN_DA, EN_AA, EN_AD
		i_bTemp = intSpiRead(ANALOG_CODEC_CTRL_9) & ~(bmBIT5 | bmBIT4 | bmBIT2 | bmBIT1);
		intSpiWrite(ANALOG_CODEC_CTRL_9, i_bTemp);
	
		EXTERN_I2S_AND_DSP = 0;
		// power down peripheral component
		GAIN_TEST_PASS |= bmBIT1;
		// power down PLL for USB and codec
		PLL_PD_DAC_DIS |= (bmBIT3 | bmBIT2);
	
		_nop_();
		_nop_();
		_nop_();
		_nop_();

		PLL_PD_DAC_DIS &= ~(bmBIT3 | bmBIT2);
		GAIN_TEST_PASS &= ~bmBIT1;
	} 

}

/**
 *
 */
void HandleInt1Isr() using 1
{
	// GPI interrupt request
	if(PERI_INT_STAT & bmBIT5)
	{
		g_bmGpiRequest = TRUE;		
		g_wGpiData = (((WORD)(GPI_DATA[1])) << 8) | GPI_DATA[0];		
		PERI_INT_STAT = bmBIT5; 		
	}
}

/**
 *
 */
void HandleTimer1Isr() using 1
{
	Timer1Config();
	++g_bTimer1Count;
}

/**
 *
 */
void HandleUartIsr() using 1
{
}

/**
 *
 */
void INT0_ISR()
{
	HandleInt0Isr();
}

/**
 *
 */
void TIMER0_ISR()
{
	HandleTimer0Isr();
}

/**
 *
 */
void INT1_ISR()
{   
	HandleInt1Isr();
}

/**
 *
 */
void TIMER1_ISR()
{
	HandleTimer1Isr();
}

/**
 *
 */
void UART_ISR()
{
	HandleUartIsr();
}