@echo off
:: SETLOCAL EnableDelayedExpansion permite evaluar las variables en ejecución
:: Por ejemplo, la variable HayError dentro de un if no se evaluaba correctamente después de llamar a ComprobarErrores
SETLOCAL EnableDelayedExpansion
title Copiar Configuración SCV
:: ----------------------------------------------------------------------------
::    Parámetros del script:
::     1. Usuario de la BD
::     2. Password del usuario
::     3. BD Origen 
::     4. BD Destino
::     5. Servidor de BD
:: ----------------------------------------------------------------------------
echo.__________________________________________________________________________
echo.
echo.  SCRIPT PARA COPIAR LA CONFIGURACIÓN SCV
echo.__________________________________________________________________________
echo.
:: ----------------------------------------------------------------------------
for /F "tokens=*" %%L in ('hostname') do set Servidor=%%L
for /F "tokens=*" %%L in ('cd') do set DirTrabajo=%%L

:: ----------------------------------------------------------------------------
:: Parámetros del script
:: ----------------------------------------------------------------------------
:PARAMETROS
if .%1 == . goto :FIN_ERROR_PARAMETROS
if .%2 == . goto :FIN_ERROR_PARAMETROS
if .%3 == . goto :FIN_ERROR_PARAMETROS
if .%4 == . goto :FIN_ERROR_PARAMETROS

Set Usuario=%1
Set Password=%2
Set BD_CD40_Origen=%3
Set BD_CD40_Destino=%4
if NOT .%5 == . set Servidor=%5

if /I .%BD_CD40_Origen% EQU .%BD_CD40_Destino% (
  echo.
  echo.    ERROR: La BD Origen y destino no pueden ser la misma >&2
  goto FIN_ERROR
)

:: Configuración de la conexión a la BD
call :ConfConexion

:INICIO
call ExportarConfiguracion.bat %Usuario% %Password% %BD_CD40_Origen% "TODO" "%DumpTempFile%" %Servidor%  2> %ErrFile%
if "%ERRORLEVEL%" NEQ "0" (
  echo.
  echo.   CopiarConfiguracion.bat: Error al ejecutar ExportarConfiguracion.bat >&2
  type %ErrFile% >>&2
  goto :FIN_ERROR
)

call ImportarConfiguracion.bat %Usuario% %Password% %BD_CD40_Destino% 1 "%DumpTempFile%" %Servidor% 2> %ErrFile%
if "%ERRORLEVEL%" NEQ "0" (
  echo.
  echo.   CopiarConfiguracion.bat: Error al ejecutar ImportarConfiguracion.bat >&2
  type %ErrFile% >>&2
  goto :FIN_ERROR
)

goto FIN_OK

:: ---------------------------------------------------------------------------
::
::            P R O C E D I M I E N T O S   D E L   S C R I P T 
::
:: ---------------------------------------------------------------------------
:: ConfConexion
:: ---------------------------------------------------------------------------
:ConfConexion
   set InfoConexion=tmpConn_%~n0.cnf
   echo.[client]             > %InfoConexion%
   echo.user=%Usuario%      >> %InfoConexion%
   echo.password=%Password% >> %InfoConexion%
   set SQLTempFile=tmp_%~n0.sql
   set DumpTempFile=tmp_dump_%~n0.sql
   set ErrFile=tmp_%~n0.err

:Fin_ConfConexion
  goto :eof

:: ---------------------------------------------------------------------------
:: ComprobarErrores: comprueba los errores en el fichero que se le pasa como parámetro
:: ---------------------------------------------------------------------------
:ComprobarErrores
  set procedimiento=%~1
  set ficheroErrores=%~2
  set HayError=N
 
  for /F "tokens=*" %%L in (%ficheroErrores%) do (
     set HayError=S
     echo.    Error %procedimiento% >&2
     type %ficheroErrores% >>&2
  )

:FinComprobarErrores
  goto :eof


:FIN_ERROR_PARAMETROS
echo.
echo.    (%date% %time%)
echo.
echo.    ERROR PARAMETROS >&2
echo.     1. Usuario de la BD >>&2
echo.     2. Password del usuario root de la BD >>&2
echo.     3. BD Origen  >>&2
echo.     4. BD Destino >>&2
echo.     5. Servidor donde se encuentra la BD >>&2

:FIN_ERROR
echo.
echo. ________________________________________________________________________
echo.
echo.    SE HA PRODUCIDO UN ERROR
echo. ________________________________________________________________________
echo.
EXIT /B 1

:FIN_OK
echo.
echo.
echo.    (%date% %time%)   Copia Finalizada
echo.
echo.
if EXIST %SQLTempFile% (del %SQLTempFile% >nul)
if EXIST %InfoConexion% (del %InfoConexion% >nul)
if EXIST %DumpTempFile% (del %DumpTempFile% >nul)
if EXIST %ErrFile% (del %ErrFile% >nul)

:FIN
ENDLOCAL
EXIT /B 0
