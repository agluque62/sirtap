@echo off
:: SETLOCAL EnableDelayedExpansion permite evaluar las variables en ejecución
:: Por ejemplo, la variable HayError dentro de un if no se evaluaba correctamente después de llamar a ComprobarErrores
SETLOCAL EnableDelayedExpansion
title Copiar Configuración SCV
:: ----------------------------------------------------------------------------
::    Parámetros del script:
::     1. Usuario de la BD
::     2. Password del usuario
::     3. Nombre de la Configuración a pasar a Online (new_cd40_trans)
::     4. Servidor de BD
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
Set BD_CD40_Offline=%3
if NOT .%4 == . set Servidor=%4
if .%4 == . set Servidor=LOCALHOST

:: Configuración de la conexión a la BD
call :ConfConexion

:INICIO
set HayError=N
call :CheckBD_Existe %BD_CD40_Offline%
if .%HayError% == .S goto FIN_ERROR

set HayError=N
call :NombreBD_Ed %BD_CD40_Offline%
if .%HayError% == .S goto FIN_ERROR
set BD_CD40_Offline_Ed=%BD_Ed%

set HayError=N
call :CheckBD_Existe %BD_CD40_Offline_Ed%
if .%HayError% == .S goto FIN_ERROR

set HayError=N
call :NombreBD_Ed_Online
if .%HayError% == .S goto FIN_ERROR
set BD_CD40_Online_Ed=%BD_Online_Ed%

echo.
echo.   BD_CD40_Offline   : %BD_CD40_Offline%
echo.   BD_CD40_Offline_Ed: %BD_CD40_Offline_Ed%
echo.   BD_CD40_Online_Ed : %BD_CD40_Online_Ed%
echo.

:COMPROBAR_ESQUEMA
echo.
echo.   Comprobar que ambas BD de edición tienen el mismo esquema
mysql --defaults-file=%InfoConexion% -h %Servidor% -f -n -N -B -e "SELECT new_cd40_offline.DiffEsquemaConfEdicion('%BD_CD40_Offline_Ed%');" > %SQLTempFile% 2> %ErrFile%
if "%ERRORLEVEL%" NEQ "0" (
  echo.
  echo.   ActivarConfiguracion.bat: Error al comprobar el esquema de %BD_CD40_Offline_Ed% >&2
  type %ErrFile% >>&2
  set HayError=S
  goto FIN_ERROR
)

:: El log no debe tener ni una sola línea
set /a NumDiff=0
for /F "tokens=*" %%L in (%SQLTempFile%) do (
   set /a NumDiff+=1
)

if %NumDiff% NEQ 0 (
  echo.
  echo.    ActivarConfiguracion.bat: El esquema de %BD_CD40_Offline_Ed% es diferente al de edición de producción >&2
  echo.    Consultar el fichero %SQLTempFile% para ver las diferencias >>&2
  goto FIN_ERROR
)

:EXPORT_TRIGGERS
echo.
echo.   Exportar los triggers de %BD_CD40_Online_Ed%
:: Exportar los triggers de la BD de producción, para importarla después de cambiar los datos de la BD de edición
call ExportarConfiguracion.bat %Usuario% %Password% %BD_CD40_Online_Ed% TRIGGERS %DumpTempFile% %Servidor% 2> %ErrFile%
if "%ERRORLEVEL%" NEQ "0" (
  echo.
  echo.   ActivarConfiguracion.bat: Error al ejecutar ExportarConfiguracion.bat solo TRIGGERS >&2
  type %ErrFile% >>&2
  goto :FIN_ERROR
)

:BORRAR_TRIGGERS
echo.
echo.   Borrar triggers de %BD_CD40_Online_Ed%
mysql --defaults-file=%InfoConexion% -h %Servidor% -f -n -N -B -e "CALL new_cd40_offline.DropTriggers('%BD_CD40_Online_Ed%')" > %SQLTempFile% 2> %ErrFile%
if "%ERRORLEVEL%" NEQ "0" (
  echo.
  echo.    ActivarConfiguracion.bat: Error al borrar los triggers de %BD_CD40_Offline_Ed% >&2
  type %ErrFile% >>&2
  goto FIN_ERROR
)

:COPIAR_DATOS
echo.
echo.   Copiar conf. %BD_CD40_Offline_Ed% a producción
mysql --defaults-file=%InfoConexion% -h %Servidor% -f -n -N -B -e "CALL new_cd40_offline.ActivarConfiguracion('%BD_CD40_Offline_Ed%')" > %SQLTempFile% 2> %ErrFile%
if "%ERRORLEVEL%" NEQ "0" (
  echo.
  echo.    ActivarConfiguracion.bat: Error al copiar los datos de la configuración offline a producción ^(BD edición^) >&2
  type %ErrFile% >>&2
  goto FIN_ERROR
)

:IMPORT_TRIGGERS
:: Solo hay que importar el código de la BD de edición, no toda la configuración, así que no se usa ImportarConfiguracion.bat
echo.
echo.   Importar los triggers de %BD_CD40_Online_Ed%
mysql --defaults-file=%InfoConexion% -h %Servidor% -e "source "%DumpTempFile%";" 2> %ErrFile% 
if "%ERRORLEVEL%" NEQ "0" (
  echo.
  echo.    ActivarConfiguracion.bat: Error al importar en %BD_CD40_Offline_Ed% los triggers >&2
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
    echo.    CheckBD_Existe_Err: La BD %BD% no está en la lista de CONFIGURACIONES >&2
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
    echo.    CheckBD_Existe_Err: La BD %BD% no existe >&2
    set HayError=S
  )

:Fin_CheckBD_Existe
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
:: NombreBD_Ed_Online
:: Consulta el nombre de la BD Online de edición (trans)
:: ---------------------------------------------------------------------------
:NombreBD_Ed_Online
  mysql --defaults-file=%InfoConexion% -h %Servidor% -f -n -N -B -e "SELECT CONF_EDICION FROM new_cd40_offline.CONFIGURACIONES WHERE TIPO = 'ONLINE'" > %SQLTempFile%
  if "%ERRORLEVEL%" NEQ "0" (
    echo.
    echo.    NombreBD_Ed_Online_Err: Error al ejecutar la sentencia para consultar el nombre de la BD de edicion de producción ^(online^)
    set HayError=S
	goto :Fin_NombreBD_Ed_Online
  )
  
  :: El log incluirá el nombre que le corresponde a la BD Edicion
  set /a NumBD=0
  for /F "tokens=*" %%L in (%SQLTempFile%) do (
     set /a NumBD+=1
	 set BD_Online_Ed=%%L
  )

  if %NumBD% NEQ 1 (
    echo.
    echo.    NombreBD_Ed_Online_Err: Error al consultar la BD de edición Online
    set HayError=S
  )

:Fin_NombreBD_Ed_Online
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
echo.     3. Nombre de la Conf. a pasar a Online ^(_trans^) >>&2
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
