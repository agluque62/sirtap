#ifndef __REGISTERS_H__
#define __REGISTERS_H__

#ifdef ALLOCATE_EXTERN
#define EXTERN
#define _AT_	_at_
#else
#define EXTERN	extern
#define _AT_	;/ ## /
#endif

EXTERN volatile BYTE xdata USB_GLOBAL_CTRL			_AT_ 0x0000;
EXTERN volatile BYTE xdata USB_ADDRESS				_AT_ 0x0001;
EXTERN volatile BYTE xdata USB_GLOBAL_INT			_AT_ 0x0002;
EXTERN volatile BYTE xdata USB_GLOBAL_INT_EN		_AT_ 0x0003;
EXTERN volatile BYTE xdata USB_EP_NUM				_AT_ 0x0004;
EXTERN volatile BYTE xdata USB_EP_CTRL_X			_AT_ 0x0005;
EXTERN volatile BYTE xdata USB_EP_STAT_X			_AT_ 0x0006;
EXTERN volatile BYTE xdata USB_EP_FIFO_RST			_AT_ 0x0007;
EXTERN volatile BYTE xdata USB_EP_INT				_AT_ 0x0008;
EXTERN volatile BYTE xdata USB_EP_INT_EN			_AT_ 0x0009;
EXTERN volatile BYTE xdata USB_EP_DATA_X			_AT_ 0x000A;

EXTERN volatile BYTE xdata ADC_DAC_FORMAT				_AT_ 0x000E;
EXTERN volatile BYTE xdata PLAY_REC_RESOLUTION			_AT_ 0x000F;

EXTERN volatile BYTE xdata PERI_INT_STAT			_AT_ 0x0010;
EXTERN volatile BYTE xdata PERI_INT_EN				_AT_ 0x0011;
EXTERN volatile BYTE xdata PERI2_INT_STAT			_AT_ 0x0012;
EXTERN volatile BYTE xdata PERI2_INT_EN				_AT_ 0x0013;

EXTERN volatile BYTE xdata ISO_OUT_FRP_L				_AT_ 0x0014;
EXTERN volatile BYTE xdata ISO_OUT_FRP_H				_AT_ 0x0015;
EXTERN volatile BYTE xdata ISO_OUT_FWP_L				_AT_ 0x0016;
EXTERN volatile BYTE xdata ISO_OUT_FWP_H				_AT_ 0x0017;
EXTERN volatile BYTE xdata ISO_OUT_FSN_L				_AT_ 0x0018;
EXTERN volatile BYTE xdata ISO_OUT_FSN_H				_AT_ 0x0019;
EXTERN volatile BYTE xdata FIFO_MAP_SETTING				_AT_ 0x0022;

EXTERN volatile BYTE xdata I2C_SLV_DATA[4]			_AT_ 0x0030;
EXTERN volatile BYTE xdata I2C_SLV_STATUS[2]		_AT_ 0x0034;
EXTERN volatile BYTE xdata I2C_SLV_MAP				_AT_ 0x0036;
EXTERN volatile BYTE xdata I2C_SLV_SYN_ACK			_AT_ 0x0037;
EXTERN volatile BYTE xdata EXT_SPI_DATA[3]			_AT_ 0x0039;
EXTERN volatile BYTE xdata EXT_SPI_INT				_AT_ 0x003E;
EXTERN volatile BYTE xdata EQ_D_BAND1_GAIN			_AT_ 0x0040;
EXTERN volatile BYTE xdata EQ_D_BAND2_GAIN			_AT_ 0x0041;
EXTERN volatile BYTE xdata EQ_D_BAND3_GAIN			_AT_ 0x0042;
EXTERN volatile BYTE xdata EQ_D_BAND4_GAIN			_AT_ 0x0043;
EXTERN volatile BYTE xdata EQ_D_BAND5_GAIN			_AT_ 0x0044;
EXTERN volatile BYTE xdata EQ_D_DIS_TEST_MODE		_AT_ 0x0045;
EXTERN volatile BYTE xdata EQ_D_BAND_NUM				_AT_ 0x0046;
EXTERN volatile BYTE xdata EQ_D_COEFF_X				_AT_ 0x0047;
EXTERN volatile BYTE xdata EQ_D_SOUND_SWITCH		_AT_ 0x0049;
EXTERN volatile BYTE xdata EQ_D_MODE_SETTING			_AT_ 0x004A;
EXTERN volatile BYTE xdata UART_BAUDRATE_SEL		_AT_ 0x0053;
EXTERN volatile BYTE xdata PLL_ADAPTIVE_EN			_AT_ 0x0056;
EXTERN volatile BYTE xdata EXT_I2S_FORMAT			_AT_ 0x0057;
EXTERN volatile BYTE xdata PLL_PD_DAC_DIS			_AT_ 0x0058;
EXTERN volatile BYTE xdata EXTERN_I2S_AND_DSP		_AT_ 0x0059;
EXTERN volatile BYTE xdata REC_SRC_TEST_SETTING		_AT_ 0x005A;

EXTERN volatile BYTE xdata DAC_VOL_CTRL[2]			_AT_ 0x005B;
EXTERN volatile BYTE xdata ADC_VOL_CTRL[2]			_AT_ 0x005D;
EXTERN volatile BYTE xdata VR_READ_AGC_CTRL			_AT_ 0x005F;

EXTERN volatile BYTE xdata SPDIF_FNUC_CTRL[2]		_AT_ 0x0060;
EXTERN volatile BYTE xdata SPDIF_IN_INT_STATUS		_AT_ 0x0062;
EXTERN volatile BYTE xdata SPDIF_OUT_STATUS[3]		_AT_ 0x0063;
EXTERN volatile BYTE xdata SPDIF_IN_STATUS[4]		_AT_ 0x0066;
EXTERN volatile BYTE xdata UART_RX_DATA		_AT_ 0x006A;
EXTERN volatile BYTE xdata UART_TX_DATA		_AT_ 0x006B;
EXTERN volatile BYTE xdata DIGITAL_MIC_SETTING		_AT_ 0x006C;

EXTERN volatile BYTE xdata EQ_A_BAND1_GAIN			_AT_ 0x0070;
EXTERN volatile BYTE xdata EQ_A_BAND2_GAIN			_AT_ 0x0071;
EXTERN volatile BYTE xdata EQ_A_BAND3_GAIN			_AT_ 0x0072;
EXTERN volatile BYTE xdata EQ_A_BAND4_GAIN			_AT_ 0x0073;
EXTERN volatile BYTE xdata EQ_A_BAND5_GAIN			_AT_ 0x0074;
EXTERN volatile BYTE xdata EQ_A_DIS_TEST_MODE		_AT_ 0x0075;
EXTERN volatile BYTE xdata EQ_A_BAND_NUM				_AT_ 0x0076;
EXTERN volatile BYTE xdata EQ_A_COEFF_X				_AT_ 0x0077;

EXTERN volatile BYTE xdata I2C_MASTER_DEST_ADDR		_AT_ 0x0080;  // Destination address of target slave device
EXTERN volatile BYTE xdata I2C_MASTER_MAP_L			_AT_ 0x0081;  // Low byte address of target slave device
EXTERN volatile BYTE xdata I2C_MASTER_MAP_H			_AT_ 0x0082;  // High byte address of target slave device
EXTERN volatile BYTE xdata I2C_MASTER_DATA[16]		_AT_ 0x0083;  // Data read from or write to target slave device
EXTERN volatile BYTE xdata I2C_MASTER_CTRL_0		_AT_ 0x0093;  // Control and status 
EXTERN volatile BYTE xdata I2C_MASTER_CTRL_1		_AT_ 0x0094;  // Control and status
EXTERN volatile BYTE xdata I2C_MASTER_CTRL_2		_AT_ 0x0095;
EXTERN volatile BYTE xdata IN_SPI_DATA[2]		_AT_ 0x0098;
EXTERN volatile BYTE xdata IN_SPI_CTRL			_AT_ 0x009A;

EXTERN volatile BYTE xdata IR_LOGIC1_CTRL[3]		_AT_ 0x00A0;
EXTERN volatile BYTE xdata IR_LOGIC0_CTRL[3]		_AT_ 0x00A3;
EXTERN volatile BYTE xdata IR_HEAD_CODE_CTRL[3]		_AT_ 0x00A6;
EXTERN volatile BYTE xdata IR_REPEAT_CODE_CTRL[3]	_AT_ 0x00A9;
EXTERN volatile BYTE xdata IR_CTRL[3]				_AT_ 0x00AC;
EXTERN volatile BYTE xdata IR_RCV_DATA[4]			_AT_ 0x00AF;
EXTERN volatile BYTE xdata IR_RC6_CTRL			_AT_ 0x00B3;

EXTERN volatile BYTE xdata USB_PHY_POW				_AT_ 0x00B4;
EXTERN volatile BYTE xdata MCU_CLK_SWITCH			_AT_ 0x00B5;
EXTERN volatile BYTE xdata MOD_CLK_CTRL[2]			_AT_ 0x00B6;
EXTERN volatile BYTE xdata MOD_RESET_CTRL			_AT_ 0x00B8;
EXTERN volatile BYTE xdata GAIN_TEST_PASS			_AT_ 0x00B9;
EXTERN volatile BYTE xdata CODEC_TEST_MONITOR			_AT_ 0x00BA;
EXTERN volatile BYTE xdata GPIO_IO_STRENGTH			_AT_ 0x00BB;
EXTERN volatile BYTE xdata TEST_MODE_THD			_AT_ 0x00BF;

EXTERN volatile BYTE xdata GPO_DATA[2]				_AT_ 0x00C0;
EXTERN volatile BYTE xdata GPI_DATA[2]				_AT_ 0x00C2;
EXTERN volatile BYTE xdata GPIO_DIR_CTRL[2]			_AT_ 0x00C4;
EXTERN volatile BYTE xdata GPIO_INT_MASK[2]			_AT_ 0x00C6;
EXTERN volatile BYTE xdata GPIO_DEBOUNCE[2]			_AT_ 0x00C8;

EXTERN volatile BYTE xdata LED_BUZZ_CTRL			_AT_ 0x00CA;

EXTERN volatile BYTE xdata LED_FREQ					_AT_ 0x00D1;
EXTERN volatile BYTE xdata LED_DUTY					_AT_ 0x00D2;
EXTERN volatile BYTE xdata LED1_ON_TIME				_AT_ 0x00D3;
EXTERN volatile BYTE xdata LED1_CYCLE_TIME			_AT_ 0x00D4;
EXTERN volatile BYTE xdata LED2_ON_TIME				_AT_ 0x00D5;
EXTERN volatile BYTE xdata LED2_CYCLE_TIME			_AT_ 0x00D6;
EXTERN volatile BYTE xdata LED3_ON_TIME				_AT_ 0x00D7;
EXTERN volatile BYTE xdata LED3_CYCLE_TIME			_AT_ 0x00D8;

EXTERN volatile BYTE xdata BONDING_OPTION			_AT_ 0x00D9;
EXTERN volatile BYTE xdata GPO_SWITCH[8]			_AT_ 0x00DA;

EXTERN volatile BYTE xdata GPI_REMOTE_WAKE[2]		_AT_ 0x00E2;

EXTERN volatile BYTE xdata GPIO_PUPD_CTRL[4]		_AT_ 0x00E4;

EXTERN volatile BYTE xdata SHARE_MCU_PORT0		_AT_ 0x00E8;
EXTERN volatile BYTE xdata AGC_GLOBAL_CTRL			_AT_ 0x00F0;
EXTERN volatile BYTE xdata AGC_TH_CTRL[3]			_AT_ 0x00F4;
EXTERN volatile BYTE xdata AGC_SIM_CTRL			_AT_ 0x00F8;
EXTERN volatile BYTE xdata AGC_MAX_GAIN			_AT_ 0x00F9;
EXTERN volatile BYTE xdata AGC_MIN_GAIN			_AT_ 0x00FA;

EXTERN volatile BYTE xdata CTRL_REG_H_ADDR		_AT_ 0x00FE;
EXTERN volatile BYTE xdata CTRL_REG_L_ADDR			_AT_ 0x00FF;

// SFR
sfr P0      = 0x80;
sfr SP      = 0x81;
sfr DPL     = 0x82;
sfr DPH     = 0x83;
sfr WDTREL	= 0x86;
sfr PCON    = 0x87;
         sbit IDLE   = 0x87+0;
         sbit STOP   = 0x87+1;
         sbit GF0    = 0x87+2;
         sbit GF1    = 0x87+3;
         sbit SMOD0  = 0x87+7;
sfr TCON    = 0x88;
         sbit IT0    = 0x88+0;
         sbit IE0    = 0x88+1;
         sbit IT1    = 0x88+2;
         sbit IE1    = 0x88+3;
         sbit TR0    = 0x88+4;
         sbit TF0    = 0x88+5;
         sbit TR1    = 0x88+6;
         sbit TF1    = 0x88+7;
sfr TMOD    = 0x89;
         sbit M00    = 0x89+0;
         sbit M10    = 0x89+1;
         sbit CT0    = 0x89+2;
         sbit GATE0  = 0x89+3;
         sbit M01    = 0x89+4;
         sbit M11    = 0x89+5;
         sbit CT1    = 0x89+6;
         sbit GATE1  = 0x89+7;
sfr TL0     = 0x8A;
sfr TL1     = 0x8B;
sfr TH0     = 0x8C;
sfr TH1     = 0x8D;
sfr CKCON	= 0x8E;
sfr P1      = 0x90;
sfr SCON    = 0x98;
         sbit RI    = 0x98+0;
         sbit TI    = 0x98+1;
         sbit RB8   = 0x98+2;
         sbit TB8   = 0x98+3;
         sbit REN   = 0x98+4;
         sbit SM2   = 0x98+5;
         sbit SM1   = 0x98+6;
         sbit SM0   = 0x98+7;
sfr SBUF    = 0x99;
sfr P2      = 0xA0;
sfr IE      = 0xA8;
         sbit EX0   = 0xA8+0;
         sbit ET0   = 0xA8+1;
         sbit EX1   = 0xA8+2;
         sbit ET1   = 0xA8+3;
         sbit ES0   = 0xA8+4;
         sbit ET2   = 0xA8+5;
         sbit ES1   = 0xA8+6;
         sbit EA    = 0xA8+7;
sfr S0RELL	= 0xAA;
sfr P3      = 0xB0;
sfr IP      = 0xB8;
         sbit PX0   = 0xB8+0;
         sbit PT0   = 0xB8+1;
         sbit PX1   = 0xB8+2;
         sbit PT1   = 0xB8+3;
         sbit PS0   = 0xB8+4;
         sbit PT2   = 0xB8+5;
         sbit PS1   = 0xB8+6;
sfr S0RELH  = 0xBA;
sfr PSW    = 0xD0;
         sbit P     = 0xD0+0;
         sbit FL    = 0xD0+1;
         sbit OV    = 0xD0+2;
         sbit RS0   = 0xD0+3;
         sbit RS1   = 0xD0+4;
         sbit F0    = 0xD0+5;
         sbit AC    = 0xD0+6;
         sbit CY    = 0xD0+7;
sfr ADCON	= 0xD8;
sfr ACC    = 0xE0;
sfr B      = 0xF0;
sfr VIDL	= 0xF8;
sfr VIDH	= 0xF9;
sfr PIDL	= 0xFA;
sfr PIDH	= 0xFB;
sfr EXCAL	= 0xFC;
sfr EXCAH	= 0xFD;
sfr EXCLL	= 0xFE;
sfr EXCLH	= 0xFF;

enum 
{
	I2S_DECODER_CTRL_0 = 0x00,		// 0x00
	I2S_DECODER_CTRL_1,				// 0x01
	I2S_ENCODER_CTRL_0,				// 0x02
	I2S_ENCODER_CTRL_1,				// 0x03
	DIGITAL_CODEC_CTRL_0,			// 0x04
	DIGITAL_CODEC_CTRL_1,			// 0x05
	ANALOG_CODEC_CTRL_0,			// 0x06
	ANALOG_CODEC_CTRL_1,			// 0x07
	ANALOG_CODEC_CTRL_2,			// 0x08
	ANALOG_CODEC_CTRL_3,			// 0x09
	ANALOG_CODEC_CTRL_4,			// 0x0A
	ANALOG_CODEC_CTRL_5,			// 0x0B	
	ANALOG_CODEC_CTRL_6,			// 0x0C
	ANALOG_CODEC_CTRL_7,			// 0x0D	
	ANALOG_CODEC_CTRL_8,			// 0x0E		
	ANALOG_CODEC_CTRL_9,			// 0x0F	
	ANALOG_CODEC_CTRL_A,			// 0x10
	ANALOG_CODEC_CTRL_B,			// 0x11
	ANALOG_CODEC_CTRL_C,			// 0x12
	ANALOG_CODEC_CTRL_D,			// 0x13
	ANALOG_CODEC_CTRL_E,			// 0x14
	IO_PAD_CTRL,					// 0x15
	DEBUG_MODE_CTRL_0,			// 0x16
	DEBUG_MODE_CTRL_1,			// 0x17
	DEBUG_MODE_CTRL_2,			// 0x18
	DEBUG_MODE_CTRL_3,			// 0x19
	ANALOG_CODEC_CTRL_F,			// 0x1A
	ANALOG_CODEC_CTRL_10			// 0x1B
};

#endif // __REGISTERS_H__
