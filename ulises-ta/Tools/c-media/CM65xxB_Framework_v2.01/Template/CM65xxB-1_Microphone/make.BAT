@echo off
SET C51INC=C:\Keil\C51\INC\Evatronix\;C:\Keil\C51\INC\
SET C51LIB=C:\Keil\C51\LIB
SET CPU_TYPE=R8051XC
SET CPU_VENDOR=Evatronix
SET UV2_TARGET=Target 1
SET CPU_XTAL=0x016E3600
SET PATH=%PATH%;C:\Keil\C51\BIN

set CONFIG=

set VER=%2

if "%CONFIG%" equ "" (
    set A51FLAGS="SET(SMALL) INCDIR(..\CM65xxB-1\inc;.\inc) EP"
    set C51FLAGS="OMF2 OPTIMIZE(9,SIZE) INCDIR(..\CM65xxB-1\inc;.\inc) INTVECTOR(0x0000) DEFINE(VER=%VER%)"
) else (
    set A51FLAGS="OMF2 SET(SMALL) INCDIR(..\CM65xxB-1\inc;.\inc) EP DEFINE(%CONFIG%)"
    set C51FLAGS="OMF2 OPTIMIZE(9,SPEED) INCDIR(..\CM65xxB-1\inc;.\inc) INTVECTOR(0x0000) DEFINE(%CONFIG%)"
)

set LIB_FILE=..\CM65xxB-1\CM65XXB-1.LIB
set OBJ_FILES=audio.obj,dscr.obj,int.obj,io.obj,main.obj,request.obj,usb.obj

del /q .\output\*.*
REM ###########################################################
REM Compile
REM ###########################################################
@echo off
FOR %%i IN (*.c) DO call c51 %%i %C51FLAGS% OBJECT(.\output\%%i.obj)
FOR %%i IN (*.a51) DO call a51 %%i %A51FLAGS% OBJECT(.\output\%%i.obj)
REM ###########################################################
REM Link
REM ###########################################################
@echo on
LX51 @cm65xx_cmd.lin
OHX51.EXE .\output\cm65xxfw HEX

..\Tools\Hex2Rom.exe .\output\cm65xxfw.hex %1_ROM.bin
move *.bin .\output
move *.lst .\output


