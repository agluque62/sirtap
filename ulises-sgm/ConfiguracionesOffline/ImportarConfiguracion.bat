@echo off
:: SETLOCAL EnableDelayedExpansion permite evaluar las variables en ejecución
:: Por ejemplo, la variable HayError dentro de un if no se evaluaba correctamente después de llamar a ComprobarErrores
SETLOCAL EnableDelayedExpansion
title Importar Conf. SCV
:: ----------------------------------------------------------------------------
::    Parámetros del script:
::     1. Usuario de la BD
::     2. Password del usuario
::     3. BD a importar 
::        Será el nombre de la BD con la conf principal.
::        El script importará tanto esa BD como la BD de edición (_trans)
::     4. Borrar: 0/1. Indica si hay que borrar la BD en caso de que exista. Por defecto 0
::     5. Fichero origen del import
::     6. Servidor donde se importará la BD
:: ----------------------------------------------------------------------------
echo.__________________________________________________________________________
echo.
echo.  SCRIPT PARA IMPORTAR LA CONF. SCV
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
Set BD_CD40_Destino=%3
if .%4 == .1 (
  set BorrarBD=1
) else (
  set BorrarBD=0
)	
if .%5 == . set FichOrigen=%DirTrabajo%\%BD_CD40_Destino%.sql
if NOT .%5 == . set FichOrigen=%5
if NOT .%6 == . set Servidor=%6

echo.   Usuario   : %Usuario%
echo.   Servidor  : %Servidor%
echo.   FichOrigen: %FichOrigen%
if .%BorrarBD% == .1 (
  echo.   BorrarBD  : SI
) else (
  echo.   BorrarBD  : NO
)	

::For %%G in ("%FichOrigen%") DO set BD_CD40_Origen=%%~nG

:: Configuración de la conexión a la BD
call :ConfConexion

:INICIO
set HayError=N
call :NombreBD_Origen %FichOrigen%
if .%HayError% == .S goto FIN_ERROR
set BD_CD40_Origen=%BD%
set BD_CD40_Origen_Ed=%BD_Ed%

echo.   
echo.   BD_CD40_Origen   : %BD_CD40_Origen%
echo.   BD_CD40_Origen_Ed: %BD_CD40_Origen_Ed%
echo.   

set HayError=N
call :CheckNombreBD %BD_CD40_Destino%
if .%HayError% == .S goto FIN_ERROR
set BD_CD40_Destino=%BD%

set HayError=N
call :NombreBD_Ed %BD_CD40_Destino%
if .%HayError% == .S goto FIN_ERROR
set BD_CD40_Destino_Ed=%BD_Ed%

echo.   
echo.   BD_CD40_Destino   : %BD_CD40_Destino%
echo.   BD_CD40_Destino_Ed: %BD_CD40_Destino_Ed%
echo.   

if NOT EXIST %FichOrigen% (
  echo.
  echo.    ERROR: El fichero %FichOrigen% no existe >&2
  goto :FIN_ERROR
)

echo.   
echo.   FichOrigen: %FichOrigen%
echo.   

type %FichOrigen% > %DumpTempFile%

:: ----------------------------------------------------------------------------
:: Cambiamos en el fichero el nombre de la BD Origen por el de la BD Destino
:: ----------------------------------------------------------------------------
:: Convertimos el juego de caracteres a utf8
powershell -ExecutionPolicy Bypass -File .\CambiarCodificacion.ps1 %DumpTempFile% "UTF8" 2> %ErrFile%
if "%ERRORLEVEL%" NEQ "0" goto FIN_ERROR
call :ComprobarErrores powershell.CambiarCodificacion %ErrFile%
if .!HayError! == .S goto FIN_ERROR

:: Cambiar el nombre de la BD Origen por el de la BD Destino
powershell -Command "(gc %DumpTempFile%) -replace '`%BD_CD40_Origen_Ed%`', '`%BD_CD40_Destino_Ed%`' | Out-File %DumpTempFile%" 2> %ErrFile%
if "%ERRORLEVEL%" NEQ "0" goto FIN_ERROR
call :ComprobarErrores powershell.Replace %ErrFile%
if .!HayError! == .S goto FIN_ERROR

:: Cambiar el nombre de la BD Origen por el de la BD Destino
powershell -Command "(gc %DumpTempFile%) -replace '`%BD_CD40_Origen%`', '`%BD_CD40_Destino%`' | Out-File %DumpTempFile%" 2> %ErrFile%
if "%ERRORLEVEL%" NEQ "0" goto FIN_ERROR
call :ComprobarErrores powershell.Replace %ErrFile%
if .!HayError! == .S goto FIN_ERROR

:: Convertimos el juego de caracteres a utf8 sin BOM
powershell -ExecutionPolicy Bypass -File .\CambiarCodificacion.ps1 %DumpTempFile% "Utf8SinBOM" 2> %ErrFile%
if "%ERRORLEVEL%" NEQ "0" goto FIN_ERROR
call :ComprobarErrores powershell.CambiarCodificacion %ErrFile%
if .!HayError! == .S goto FIN_ERROR


:: ----------------------------------------------------------------------------
:: Proceso de importación en las BD destino
:: ----------------------------------------------------------------------------
mysql --defaults-file=%InfoConexion% -h %Servidor% -f -n -N -B -e "CALL new_cd40_offline.AddConfiguracion('%BD_CD40_Destino%', '', %BorrarBD%)" > "%SQLTempFile%" 2> %ErrFile%
if "%ERRORLEVEL%" NEQ "0" (
  echo.
  echo.    ERROR al ejecutar new_cd40_offline.AddConfiguracion^('%BD_CD40_Destino%'^) >&2
  type %ErrFile% >>&2
  goto FIN_ERROR
)

:: Importamos las BD
mysql --defaults-file=%InfoConexion% -h %Servidor% -e "source "%DumpTempFile%";" 2> %ErrFile% 
if "%ERRORLEVEL%" NEQ "0" (
  echo.
  echo.    ERROR al importar en %BD_CD40_Destino% la info de %DumpTempFile% >&2
  type %ErrFile% >>&2
  goto FIN_ERROR
)

:: Borrar Historicos
mysql --defaults-file=%InfoConexion% -h %Servidor% -f -n -N -B -e "CALL new_cd40_offline.DelHistoricos('%BD_CD40_Destino%')" > "%SQLTempFile%" 2> %ErrFile%
if "%ERRORLEVEL%" NEQ "0" (
  echo.
  echo.    ERROR al borrar el histórico de incidencias de la conf %BD_CD40_Destino% >&2
  type %ErrFile% >>&2
  goto FIN_ERROR
)

:: Borrar Historicos
mysql --defaults-file=%InfoConexion% -h %Servidor% -f -n -N -B -e "CALL new_cd40_offline.CopyTabla('%BD_CD40_Destino%', 'OPERADORES')" > %SQLTempFile% 2> %ErrFile%
if "%ERRORLEVEL%" NEQ "0" (
  echo.
  echo.    ERROR al copiar los operadores del servidor en la conf %BD_CD40_Destino% >&2
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
   set DumpTempFile=tmp_dump_%~n0.sql
   set ErrFile=tmp_%~n0.err

:Fin_ConfConexion
  goto :eof

:: ---------------------------------------------------------------------------
:: NombreBD_Origen
:: ---------------------------------------------------------------------------
:NombreBD_Origen 
  set Fichero=%~1
  for /F "tokens=1,2" %%L in (%Fichero%) do (
    if /I "%%L" == "USE" (
      set BD=%%M
	  goto :NombreBD_Origen_Replace
	)
  )

:NombreBD_Origen_Replace 
  for /F "tokens=1,2 delims=`" %%L in ("%BD%") do (
    set BD=%%L
  )

:NombreBD_Origen_Check 
  :: Obtener el nombre que le corresponde a la BD origen
  mysql --defaults-file=%InfoConexion% -h %Servidor% -f -n -N -B -e "SELECT new_cd40_offline.GetNombreConf('%BD%')" > %SQLTempFile% 2> %ErrFile%
  if "%ERRORLEVEL%" NEQ "0" (
    echo.
    echo.    NombreBD_Origen: Error al ejecutar la sentencia para consultar el nombre de la BD ^(%BD%^) >&2
    type %ErrFile% >>&2
    set HayError=S
	goto :Fin_NombreBD_Origen
  )
  

  :: El log incluirá el nombre que le corresponde a la BD
  set /a NumBD=0
  for /F "tokens=*" %%L in (%SQLTempFile%) do (
     set /a NumBD+=1
	 set BD=%%L
  )

  if %NumBD% NEQ 1 (
    echo.
    echo.    NombreBD_Origen: Error al consultar el nombre de la BD %BD% >&2
    set HayError=S
	goto :Fin_NombreBD_Origen
  )

  :: Obtener el nombre que le corresponde a la BD de edición
  mysql --defaults-file=%InfoConexion% -h %Servidor% -f -n -N -B -e "SELECT new_cd40_offline.GetNombreConfEdicion('%BD%')" > %SQLTempFile% 2> %ErrFile%
  if "%ERRORLEVEL%" NEQ "0" (
    echo.
    echo.    NombreBD_Origen: Error al ejecutar la sentencia para consultar el nombre de la BD ^(%BD%^) >&2
    type %ErrFile% >>&2
    set HayError=S
	goto :Fin_NombreBD_Origen
  )

  :: El log incluirá el nombre que le corresponde a la BD
  set /a NumBD=0
  for /F "tokens=*" %%L in (%SQLTempFile%) do (
     set /a NumBD+=1
	 set BD_Ed=%%L
  )

  if %NumBD% NEQ 1 (
    echo.
    echo.    NombreBD_Origen: Error al consultar la BD _trans de %BD% >&2
    set HayError=S
	goto :Fin_NombreBD_Origen
  )

:Fin_NombreBD_Origen 
  goto :eof

:: ---------------------------------------------------------------------------
:: CheckNombreBD
:: Comprobar que el nombre de la nueva conf cumple la norma
:: ---------------------------------------------------------------------------
:CheckNombreBD
  set BD=%~1
  :: Obtenemos el nombre que le corresponde a la BD/configuración, que cumpla la norma de new_cd40_*
  mysql --defaults-file=%InfoConexion% -h %Servidor% -f -n -N -B -e "SELECT new_cd40_offline.GetNombreConf('%BD%')" > %SQLTempFile% 2> %ErrFile%
  if "%ERRORLEVEL%" NEQ "0" (
    echo.
    echo.    CheckNombreBD_Err: Error al ejecutar la sentencia para consultar el nombre que le corresponde a la conf %BD% >&2
    type %ErrFile% >>&2
    set HayError=S
	goto :Fin_CheckNombreBD
  )
  
  :: El log incluirá el nombre que le corresponde a la BD
  set /a NumBD=0
  for /F "tokens=*" %%L in (%SQLTempFile%) do (
     set /a NumBD+=1
	 set BD=%%L
  )

  if %NumBD% NEQ 1 (
    echo.
    echo.    CheckNombreBD_Err: Error al consultar la BD de %BD% >&2
    set HayError=S
  )

:Fin_CheckNombreBD
  goto :eof

:: ---------------------------------------------------------------------------
:: NombreBD_Ed: Consulta el nombre que le corresponde a la BD de Edición
:: ---------------------------------------------------------------------------
:NombreBD_Ed
  set BD=%~1
  :: Obtener el nombre que le corresponde a la BD de edición
  mysql --defaults-file=%InfoConexion% -h %Servidor% -f -n -N -B -e "SELECT new_cd40_offline.GetNombreConfEdicion('%BD%')" > %SQLTempFile% 2> %ErrFile%
  if "%ERRORLEVEL%" NEQ "0" (
    echo.
    echo.    NombreBD_Ed_Err: Error al ejecutar la sentencia para consultar la BD de edición ^(_trans^) de %BD% >&2
    type %ErrFile% >>&2
    set HayError=S
	goto :Fin_NombreBD_Ed
  )

  :: El log incluirá el nombre que le corresponde a la BD de edicion
  set /a NumBD=0
  for /F "tokens=*" %%L in (%SQLTempFile%) do (
     set /a NumBD+=1
	 set BD_Ed=%%L
  )

  if %NumBD% NEQ 1 (
    echo.
    echo.    NombreBD_Ed_Err: Error al consultar la BD _trans de %BD% >&2
    set HayError=S
	goto :Fin_NombreBD_Ed
  )

:Fin_NombreBD_Ed
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
echo.     2. Password del usuario >>&2
echo.     3. BD sobre la que importar  >>&2 
echo.     4. Borrar: 0/1. Indica si hay que borrar la BD en caso de que exista. Por defecto 0 >>&2
echo.     5. Fichero origen del import >>&2
echo.     6. Servidor donde se encuentra la BD >>&2  

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
echo.    (%date% %time%)   Import finalizado
echo.
echo.
if EXIST %SQLTempFile% (del %SQLTempFile% >nul)
if EXIST %InfoConexion% (del %InfoConexion% >nul)
if EXIST %DumpTempFile% (del %DumpTempFile% >nul)
if EXIST %ErrFile% (del %ErrFile% >nul)

:FIN
ENDLOCAL
EXIT /B 0

:FIN_DEPURACION
