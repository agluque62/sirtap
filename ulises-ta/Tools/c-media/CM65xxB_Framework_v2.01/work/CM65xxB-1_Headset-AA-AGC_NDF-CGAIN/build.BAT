@echo off

if "%1" == "" (
    echo [Usage] : %0 {Version}
    echo Example : %0 0921
    goto exit
)

rem del stdout.txt

call make CM65xxB-1_%1 0x%1

:exit

@echo on
