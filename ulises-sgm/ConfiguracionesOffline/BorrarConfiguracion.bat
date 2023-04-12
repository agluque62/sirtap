@echo off
:: SETLOCAL EnableDelayedExpansion permite evaluar las variables en ejecución
:: Por ejemplo, la variable HayError dentro de un if no se evaluaba correctamente después de llamar a ComprobarErrores
SETLOCAL EnableDelayedExpansion
title Borrar Configuración SCV
:: ----------------------------------------------------------------------------
::    Parámetros del script:
::     1. Usuario de la BD
::     2. Password del usuario
::     3. Nombre de la Configuración a borrar
::     4. Servidor de BD
:: ----------------------------------------------------------------------------
echo.__________________________________________________________________________
echo.
echo.  SCRIPT PARA BORRAR LA CONFIGURACIÓN SCV
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

Set Usuario=%1
Set Password=%2
Set BD_CD40_Borrar=%3
if NOT .%4 == . set Servidor=%4

:: Configuración de la conexión a la BD
call :ConfConexion

:INICIO
set HayError=N
call :CheckBD_Existe %BD_CD40_Borrar%
if .%HayError% == .S goto FIN_ERROR

set HayError=N
call :Check_No_Online  %BD_CD40_Borrar%
if .%HayError% == .S goto FIN_ERROR

echo.
echo.   BD_CD40_Borrar  : %BD_CD40_Borrar%
echo.

:BORRAR_CONF
mysql --defaults-file=%InfoConexion% -h %Servidor% -f -n -N -B -e "CALL new_cd40_offline.DelConfiguracion ('%BD_CD40_Borrar%')" > %SQLTempFile% 2> %ErrFile%
if "%ERRORLEVEL%" NEQ "0" (
  echo.
  echo.    BorrarConfiguracion.bat: Error al ejecutar el procedimiento DelConfiguracion ^('%BD_CD40_Borra%'^) >&2
  type %ErrFile% >>&2
  goto FIN_ERROR
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
   set ErrFile=tmp_%~n0.err

:Fin_ConfConexion
  goto :eof

:: ---------------------------------------------------------------------------
:: CheckBD_Existe: Comprueba que la BD existe
:: ---------------------------------------------------------------------------
:CheckBD_Existe
  set BD=%~1
  :: Consultamos si la BD / Configuración está en la lista de configuraciones
  mysql --defaults-file=%InfoConexion% -h %Servidor% -f -n -N -B -e "SELECT NOMBRE FROM new_cd40_offline.CONFIGURACIONES WHERE NOMBRE = '%BD%' OR CONF_EDICION = '%BD%'" > %SQLTempFile% 2> %ErrFile%
  if "%ERRORLEVEL%" NEQ "0" (
    echo.
    echo.    CheckBD_Existe_Err: Error al consultar la BD %BD% en la lista de CONFIGURACIONES >&2
    type %ErrFile% >>&2
    set HayError=S
	goto :Fin_CheckBD_Existe
  )
  
  :: El log debe tener 1 líneas con el nombre de la BD
  set /a NumBD=0
  for /F "tokens=*" %%L in (%SQLTempFile%) do (
     set /a NumBD+=1
  )

  if %NumBD% NEQ 1 (
    echo.
    echo.    CheckBD_Existe_Err: La BD %BD% no está en la lista de CONFIGURACIONES
  )

  :: Consultamos si la BD / Configuración existe como BD
  mysql --defaults-file=%InfoConexion% -h %Servidor% -f -n -N -B -e "SELECT SCHEMA_NAME FROM INFORMATION_SCHEMA.SCHEMATA WHERE SCHEMA_NAME = '%BD%'" > %SQLTempFile% 2> %ErrFile%
  if "%ERRORLEVEL%" NEQ "0" (
    echo.
    echo.    CheckBD_Existe_Err: Error al consultar la BD %BD% en la lista de esquemas del servidor ^(INFORMATION_SCHEMA.SCHEMATA^) >&2
    type %ErrFile% >>&2
    set HayError=S
	goto :Fin_CheckBD_Existe
  )

  :: El log debe tener 1 líneas con el nombre de la BD
  set /a NumBD=0
  for /F "tokens=*" %%L in (%SQLTempFile%) do (
     set /a NumBD+=1
  )

  if %NumBD% NEQ 1 (
    echo.
    echo.    CheckBD_Existe_Err: La BD %BD% no existe
	:: No tratamos el error si no existe. En cualquier caso vamos a borrarla
    :: set HayError=S
  )

:Fin_CheckBD_Existe
  goto :eof


:: ---------------------------------------------------------------------------
:: Check_No_Online
:: Comprueba que la BD no es Online
:: ---------------------------------------------------------------------------
:Check_No_Online
  set BD=%~1
  mysql --defaults-file=%InfoConexion% -h %Servidor% -f -n -N -B -e "SELECT NOMBRE FROM new_cd40_offline.CONFIGURACIONES WHERE TIPO = 'ONLINE' AND NOMBRE = '%BD%'" > %SQLTempFile% 2> %ErrFile%
  if "%ERRORLEVEL%" NEQ "0" (
    echo.
    echo.    Check_No_Online_Err: Error al ejecutar la sentencia para consultar el nombre de la BD de producción ^(online^) >&2
    type %ErrFile% >>&2
    set HayError=S
	goto :Fin_Check_No_Online
  )
  
  :: El log no debería incluir ningún nombre
  set /a NumBD=0
  for /F "tokens=*" %%L in (%SQLTempFile%) do (
     set /a NumBD+=1
  )

  if %NumBD% NEQ 0 (
    echo.
    echo.    Check_No_Online_Err: Se trata de la conf Online, no se puede borrar desde este script >&2
    set HayError=S
  )

:Fin_Check_No_Online
  goto :eof


:FIN_ERROR_PARAMETROS
echo.
echo.    (%date% %time%)
echo.
echo.    ERROR PARAMETROS >&2
echo.     1. Usuario de la BD >>&2
echo.     2. Password del usuario root de la BD >>&2
echo.     3. Nombre Nueva conf >>&2
echo.     4. Servidor donde se encuentra la BD >>&2  

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
echo.    (%date% %time%)   Configuración borrada
echo.
echo.
if EXIST %SQLTempFile% (del %SQLTempFile% >nul)
if EXIST %InfoConexion% (del %InfoConexion% >nul)
if EXIST %ErrFile% (del %ErrFile% >nul)

:FIN
ENDLOCAL
EXIT /B 0
