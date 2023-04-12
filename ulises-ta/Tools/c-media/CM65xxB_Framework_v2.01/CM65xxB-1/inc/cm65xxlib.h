#ifndef __CM65xxLIB_H__
#define __CM65xxLIB_H__
#include "types.h"

WORD code FeedbakTbl[9];

void UartBaudRateInit();
void I2cMasterRead();
void I2cMasterWrite();
void ModIrReset(BYTE type);
BOOL HandleI2cSlave();
BOOL HandleSpiSlave();


#endif // __CM65xxLIB_H__
