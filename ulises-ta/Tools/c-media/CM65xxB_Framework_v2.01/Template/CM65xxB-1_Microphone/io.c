#include "types.h"
#include "registers.h"

/*********************************************************************
* External library variable declaration
*********************************************************************/

/*********************************************************************
* Global/External variable definition
*********************************************************************/

/*********************************************************************
* Global/External function definition
*********************************************************************/

/*********************************************************************
* code area
*********************************************************************/
void PeriIntInit()
{
	// these peripheral int. are connected to MCU ext. int. 1
	PERI_INT_STAT = 0xFF;		// clear flags
	PERI_INT_EN = bmBIT5;
	/* bit0:IR int. enable
	   bit1:i2c master int. enable
	   bit2:i2c slave int. enable
	   bit3:spi master int. enable
	   bit4:spi slave int. enable
	   bit5:gpi int. enable
	   bit6:VR int. enable
	*/
	
	PERI2_INT_STAT = 0xFF;	// clear flags
	PERI2_INT_EN = 0;
	/* bit0:spdif sense int. enable
	   bit1:spdif lock int. enable
	   bit2:spdif rate int. enable
	   bit3:internal spi int. enable
	   bit4:baudrate int. enable
	   bit5:UART Tx int. enable
	   bit6:SE1 int. enable
	*/
}