@echo off
:: SETLOCAL EnableDelayedExpansion permite evaluar las variables en ejecución
:: Por ejemplo, la variable HayError dentro de un if no se evaluaba correctamente después de llamar a ComprobarErrores
SETLOCAL EnableDelayedExpansion
title Nueva Configuración SCV
:: ----------------------------------------------------------------------------
::    Parámetros del script:
::     1. Usuario de la BD
::     2. Password del usuario
::     3. Nombre Nueva Configuración
::     4. Servidor de BD
:: ----------------------------------------------------------------------------
echo.__________________________________________________________________________
echo.
echo.  SCRIPT PARA NUEVA LA CONFIGURACIÓN SCV
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
Set BD_CD40_Nueva=%3
if NOT .%4 == . set Servidor=%4

:: Configuración de la conexión a la BD
call :ConfConexion

:INICIO
set HayError=N
call :CheckNombreBD %BD_CD40_Nueva%
if .%HayError% == .S goto FIN_ERROR
set BD_CD40_Nueva=%BD%

set HayError=N
call :NombreBD_Ed %BD_CD40_Nueva%
if .%HayError% == .S goto FIN_ERROR
set BD_CD40_Nueva_Ed=%BD_Ed%

set HayError=N
call :NombreBD_Online
if .%HayError% == .S goto FIN_ERROR
set BD_CD40_Online=%BD_Online%

echo.
echo.   BD_CD40_Nueva   : %BD_CD40_Nueva%
echo.   BD_CD40_Nueva_Ed: %BD_CD40_Nueva_Ed%
echo.   BD_CD40_Online  : %BD_CD40_Online%
echo.

:: Comprobar que la BD/Configuración no existe
set HayError=N
call :CheckBD_NoExiste %BD_CD40_Nueva%
if .%HayError% == .S goto FIN_ERROR

:EXPORT_TABLAS
:: Exportar la configuración Online, pero solo las tablas y vistas, sin los datos ni el código, para evitar que los triggers se disparen en la carga de los datos iniciales
call ExportarConfiguracion.bat %Usuario% %Password% %BD_CD40_Online% TABLAS %DumpTempFile% %Servidor% 2> %ErrFile%
if "%ERRORLEVEL%" NEQ "0" (
  echo.
  echo.   NuevaConfiguracion.bat: Error al ejecutar ExportarConfiguracion.bat solo TABLAS >&2
  type %ErrFile% >>&2
  goto :FIN_ERROR
)

:IMPORT_TABLAS
:: Importar la configuración que se acaba de exportar
call ImportarConfiguracion.bat %Usuario% %Password% %BD_CD40_Nueva% 1 %DumpTempFile% %Servidor% 2> %ErrFile%
if "%ERRORLEVEL%" NEQ "0" (
  echo.
  echo.   NuevaConfiguracion.bat: Error al ejecutar ImportarConfiguracion.bat solo TABLAS >&2
  type %ErrFile% >>&2
  goto :FIN_ERROR
)

:DATOS_INICIALES
echo.
echo.   Copiar en ambos esquemas los datos iniciales
echo.      - Copiar datos en %BD_CD40_Nueva%
mysql --defaults-file=%InfoConexion% -h %Servidor% -f -n -N -B -e "CALL new_cd40_offline.CopyDatosIniciales ('%BD_CD40_Nueva%', 1)" > %SQLTempFile% 2> %ErrFile%
if "%ERRORLEVEL%" NEQ "0" (
  echo.
  echo.    NuevaConfiguracion.bat: Error al Copiar los Datos Iniciales en %BD_CD40_Nueva% >&2
  type %ErrFile% >>&2
  goto FIN_ERROR
)

echo.      - Copiar datos en %BD_CD40_Nueva_Ed%
mysql --defaults-file=%InfoConexion% -h %Servidor% -f -n -N -B -e "CALL new_cd40_offline.CopyDatosIniciales ('%BD_CD40_Nueva_Ed%', 1)" > %SQLTempFile% 2> %ErrFile%
if "%ERRORLEVEL%" NEQ "0" (
  echo.
  echo.    NuevaConfiguracion.bat: Error al Copiar los Datos Iniciales en %BD_CD40_Nueva_Ed% >&2
  type %ErrFile% >>&2
  goto FIN_ERROR
)

:EXPORT_CODIGO
echo.
echo.   Exportar e importar el código de la conf Online
:: Exportar el código de la configuración Online
call ExportarConfiguracion.bat %Usuario% %Password% %BD_CD40_Online% CODIGO %DumpTempFile% %Servidor%  2> %ErrFile%
if "%ERRORLEVEL%" NEQ "0" (
  echo.
  echo.   NuevaConfiguracion.bat: Error al ejecutar ExportarConfiguracion.bat solo CODIGO >&2
  type %ErrFile% >>&2
  goto :FIN_ERROR
)

:IMPORT_CODIGO
:: Importar la configuración que se acaba de exportar
call ImportarConfiguracion.bat %Usuario% %Password% %BD_CD40_Nueva% 0 %DumpTempFile% %Servidor%  2> %ErrFile%
if "%ERRORLEVEL%" NEQ "0" (
  echo.
  echo.   NuevaConfiguracion.bat: Error al ejecutar ImportarConfiguracion.bat solo CODIGO >&2
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
  
  :: El log incluirá el nombre que le corresponde a la BD Edicion
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
:: NombreBD_Online
:: Consulta el nombre de la BD Online
:: ---------------------------------------------------------------------------
:NombreBD_Online
  mysql --defaults-file=%InfoConexion% -h %Servidor% -f -n -N -B -e "SELECT NOMBRE FROM new_cd40_offline.CONFIGURACIONES WHERE TIPO = 'ONLINE'" > %SQLTempFile% 2> %ErrFile%
  if "%ERRORLEVEL%" NEQ "0" (
    echo.
    echo.    NombreBD_Online_Err: Error al ejecutar la sentencia para consultar el nombre de la BD de producción ^(online^) >&2
    type %ErrFile% >>&2
    set HayError=S
	goto :Fin_NombreBD_Online
  )
  
  :: El log incluirá el nombre que le corresponde a la BD Edicion
  set /a NumBD=0
  for /F "tokens=*" %%L in (%SQLTempFile%) do (
     set /a NumBD+=1
	 set BD_Online=%%L
  )

  if %NumBD% NEQ 1 (
    echo.
    echo.    NombreBD_Online_Err: Error al consultar la BD Online >&2
    set HayError=S
  )

:Fin_NombreBD_Online
  goto :eof


:: ---------------------------------------------------------------------------
:: CheckBD_NoExiste
:: Comprobar si la BD existe
:: ---------------------------------------------------------------------------
:CheckBD_NoExiste
  set BD=%~1
  :: Consultamos si la BD / Configuración está en la lista de configuraciones
  mysql --defaults-file=%InfoConexion% -h %Servidor% -f -n -N -B -e "SELECT NOMBRE FROM new_cd40_offline.CONFIGURACIONES WHERE NOMBRE = '%BD%' OR CONF_EDICION = '%BD%'" > %SQLTempFile% 2> %ErrFile%
  if "%ERRORLEVEL%" NEQ "0" (
    echo.
    echo.    CheckBD_NoExiste_Err: Error al consultar la BD %BD% en la lista de CONFIGURACIONES >&2
    type %ErrFile% >>&2
    set HayError=S
	goto :Fin_CheckBD_NoExiste
  )
  
  :: El log no debería incluir ningún nombre
  set /a NumBD=0
  for /F "tokens=*" %%L in (%SQLTempFile%) do (
     set /a NumBD+=1
	 set BD_Online=%%L
  )

  if %NumBD% NEQ 0 (
    echo.
    echo.    CheckBD_NoExiste_Err: La BD %BD% ya está en la lista de CONFIGURACIONES
  )

  :: Consultamos si la BD / Configuración existe como BD
  mysql --defaults-file=%InfoConexion% -h %Servidor% -f -n -N -B -e "SELECT SCHEMA_NAME FROM INFORMATION_SCHEMA.SCHEMATA WHERE SCHEMA_NAME = '%BD%'" > %SQLTempFile% 2> %ErrFile%
  if "%ERRORLEVEL%" NEQ "0" (
    echo.
    echo.    ERROR: Error al consultar la BD %BD% en la lista de esquemas del servidor ^(INFORMATION_SCHEMA.SCHEMATA^) >&2
    type %ErrFile% >>&2
    set HayError=S
	goto :Fin_CheckBD_NoExiste
  )
  
  :: El log no debería incluir ningún nombre
  set /a NumBD=0
  for /F "tokens=*" %%L in (%SQLTempFile%) do (
     set /a NumBD+=1
  )

  if %NumBD% NEQ 0 (
    echo.
    echo.    CheckBD_NoExiste_Err: La BD %BD% ya existe en el servidor >&2
    set HayError=S
	goto :Fin_CheckBD_NoExiste
  )

:Fin_CheckBD_NoExiste
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
echo.    (%date% %time%)   Nueva Configuración creada
echo.
echo.
if EXIST %SQLTempFile% (del %SQLTempFile% >nul)
if EXIST %InfoConexion% (del %InfoConexion% >nul)
if EXIST %DumpTempFile% (del %DumpTempFile% >nul)
if EXIST %ErrFile% (del %ErrFile% >nul)
 
:FIN
ENDLOCAL
EXIT /B 0
