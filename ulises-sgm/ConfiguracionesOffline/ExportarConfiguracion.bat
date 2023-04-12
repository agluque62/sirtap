@echo off
:: SETLOCAL EnableDelayedExpansion permite evaluar las variables en ejecución
:: Por ejemplo, la variable HayError dentro de un if no se evaluaba correctamente después de llamar a ComprobarErrores
SETLOCAL EnableDelayedExpansion
title Exportar Conf. SCV
:: ----------------------------------------------------------------------------
::    Parámetros del script:
::     1. Usuario de la BD
::     2. Password del usuario
::     3. BD a exportar 
::        Será el nombre de la BD con la conf principal.
::        El script exportará tanto esa BD como la BD de edición (_trans)
::     4. Modo export: Indicará el tipo de export. Los posibles valores son:
::        DATOS   : Solo exporta los datos
::        ESQUEMA : Solo exporta los objetos de la BD sin los datos de las tablas
::        TABLAS  : Solo exporta las tablas y las vistas
::        CODIGO  : Solo exporta el código de la BD, triggers y rutinas
::        TRIGGERS: Solo exporta los triggers de la BD
::        TODO    : hace un export completo (por defecto)
::     5. Fichero destino del export
::     6. Servidor donde se encuentra la BD
:: ----------------------------------------------------------------------------
echo.__________________________________________________________________________
echo.
echo.  SCRIPT PARA EXPORTAR LA CONF. SCV
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
Set BD_CD40=%3
if /I "%4" == "DATOS" (
  set TipoExport=DATOS
) else (
  if /I "%4" == "ESQUEMA" (
    set TipoExport=ESQUEMA
  ) else (
    if /I "%4" == "TABLAS" (
      set TipoExport=TABLAS
    ) else (
      if /I "%4" == "CODIGO" (
        set TipoExport=CODIGO
      ) else (
        if /I "%4" == "TRIGGERS" (
          set TipoExport=TRIGGERS
        ) else (
          set TipoExport=TODO
        )
      )
    )
  )
)

:: El parámetro %5 lo comprobamos después de comprobar la BD
if NOT .%6 == . set Servidor=%6

echo.   Usuario      : %Usuario%
echo.   Base de datos: %BD_CD40%
echo.   Export       : %TipoExport%
echo.   Servidor     : %Servidor%

:: Configuración de la conexión a la BD
call :ConfConexion

:INICIO
set HayError=N
call :NombreBD %BD_CD40%
if .%HayError% == .S goto FIN_ERROR
set BD_CD40=%BD%

set HayError=N
call :CheckBD_Existe %BD_CD40%
if .%HayError% == .S goto FIN_ERROR

:: Comprobamos el nombre del fichero y el directorio
:: Lo hacemos después de comprobar la BD para poder 
:: darle un nombre por defecto al fichero de salida
call :CheckFichero %5
echo.   Fichero      : %FichDestino%

set HayError=N
call :NombreBD_Ed %BD_CD40%
if .%HayError% == .S goto FIN_ERROR
set BD_CD40_Ed=%BD_Ed%

set HayError=N
call :CheckBD_Existe %BD_CD40_Ed%
if .%HayError% == .S goto FIN_ERROR

echo.   
echo.   BD_CD40   : %BD_CD40%
echo.   BD_CD40_Ed: %BD_CD40_Ed%
echo.   

:EXPORT
:: Exportamos sólo los datos
if /I "%TipoExport%" == "DATOS" (
  mysqldump --defaults-file=%InfoConexion% -h %Servidor% --skip-triggers -t --databases %BD_CD40% %BD_CD40_Ed% > "%FichDestino%" 2> %ErrFile%
  if "%ERRORLEVEL%" NEQ "0" goto FIN_ERROR
  call :ComprobarErrores mysqldump %ErrFile%
  if .!HayError! == .S goto FIN_ERROR
  goto :FIN_EXPORT
)

:: Exportamos sólo el esquema
if /I "%TipoExport%" == "ESQUEMA" (
  mysqldump --defaults-file=%InfoConexion% -h %Servidor% --no-data -R --databases %BD_CD40% %BD_CD40_Ed% > "%FichDestino%" 2> %ErrFile%
  if "%ERRORLEVEL%" NEQ "0" goto FIN_ERROR
  call :ComprobarErrores Exportar %ErrFile%
  if .!HayError! == .S goto FIN_ERROR
  goto :FIN_EXPORT
)

:: Exportamos sólo las tablas
if /I "%TipoExport%" == "TABLAS" (
  mysqldump --defaults-file=%InfoConexion% -h %Servidor% --no-data --skip-triggers --databases %BD_CD40% %BD_CD40_Ed% > "%FichDestino%" 2> %ErrFile%
  if "%ERRORLEVEL%" NEQ "0" goto FIN_ERROR
  call :ComprobarErrores Exportar %ErrFile%
  if .!HayError! == .S goto FIN_ERROR
  goto :FIN_EXPORT
)

:: Exportamos sólo el código
if /I "%TipoExport%" == "CODIGO" (
  mysqldump --defaults-file=%InfoConexion% -h %Servidor% --no-data --no-create-db --no-create-info --routines --triggers --skip-opt --lock-tables --databases %BD_CD40% %BD_CD40_Ed% > "%FichDestino%" 2> %ErrFile%
  if "%ERRORLEVEL%" NEQ "0" goto FIN_ERROR
  call :ComprobarErrores Exportar %ErrFile%
  if .!HayError! == .S goto FIN_ERROR
  goto :FIN_EXPORT
)

:: Exportamos sólo los triggers
if /I "%TipoExport%" == "TRIGGERS" (
  mysqldump --defaults-file=%InfoConexion% -h %Servidor% --no-data --no-create-db --no-create-info --triggers --skip-opt --lock-tables --databases %BD_CD40% %BD_CD40_Ed% > "%FichDestino%" 2> %ErrFile%
  if "%ERRORLEVEL%" NEQ "0" goto FIN_ERROR
  call :ComprobarErrores Exportar %ErrFile%
  if .!HayError! == .S goto FIN_ERROR
  goto :FIN_EXPORT
)

:: Exportamos TODA la BD
if /I "%TipoExport%" == "TODO" (
  mysqldump --defaults-file=%InfoConexion% -h %Servidor% -R --databases %BD_CD40% %BD_CD40_Ed%  > "%FichDestino%" 2> %ErrFile%
  if "%ERRORLEVEL%" NEQ "0" goto FIN_ERROR
  call :ComprobarErrores Exportar %ErrFile%
  if .!HayError! == .S goto FIN_ERROR
  goto :FIN_EXPORT
)

:FIN_EXPORT
:: Convertimos el juego de caracteres a utf8
powershell -ExecutionPolicy Bypass -File .\CambiarCodificacion.ps1 %FichDestino% "UTF8" 2> %ErrFile%
if "%ERRORLEVEL%" NEQ "0" goto FIN_ERROR
call :ComprobarErrores CambiarCodificacionUTF8 %ErrFile%
if .!HayError! == .S goto FIN_ERROR

:: Quitar el nombre de la BD Origen de los triggers en el fichero de salida
powershell -Command "(gc %FichDestino%) -replace '`%BD_CD40_Ed%`\.', '' | Out-File %FichDestino%" 2> %ErrFile%
if "%ERRORLEVEL%" NEQ "0" goto FIN_ERROR
call :ComprobarErrores Replace %ErrFile%
if .!HayError! == .S goto FIN_ERROR

powershell -Command "(gc %FichDestino%) -replace '%BD_CD40_Ed%\.', ''   | Out-File %FichDestino%" 2> %ErrFile%
if "%ERRORLEVEL%" NEQ "0" goto FIN_ERROR
call :ComprobarErrores Replace %ErrFile%
if .!HayError! == .S goto FIN_ERROR

powershell -Command "(gc %FichDestino%) -replace '`%BD_CD40%`\.', ''    | Out-File %FichDestino%" 2> %ErrFile%
if "%ERRORLEVEL%" NEQ "0" goto FIN_ERROR
call :ComprobarErrores Replace %ErrFile%
if .!HayError! == .S goto FIN_ERROR

powershell -Command "(gc %FichDestino%) -replace '%BD_CD40%\.', ''      | Out-File %FichDestino%" 2> %ErrFile%
if "%ERRORLEVEL%" NEQ "0" goto FIN_ERROR
call :ComprobarErrores Replace %ErrFile%
if .!HayError! == .S goto FIN_ERROR


:: Convertimos el juego de caracteres a utf8 sin BOM
powershell -ExecutionPolicy Bypass -File .\CambiarCodificacion.ps1 %FichDestino% "Utf8SinBOM" 2> %ErrFile%
if "%ERRORLEVEL%" NEQ "0" goto FIN_ERROR
call :ComprobarErrores CambiarCodificacionUTF8SinBOM %ErrFile%
if .!HayError! == .S goto FIN_ERROR

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
:: CheckFichero
:: ---------------------------------------------------------------------------
:CheckFichero
   set Fich=%~f1

   if .%Fich% == . set Fich=%DirTrabajo%\%BD_CD40%.sql

   for %%i in ("%DirTrabajo%") do (
     set WDDrive=%%~di
     set WDPath=%%~pi
   )
  
   for %%i in ("%Fich%") do (
     set FichDrive=%%~di
     set FichPath=%%~pi
     set FichNombre=%%~ni
     set FichExtension=%%~xi
   )

   if .%FichDrive% == . set FichDrive=%WDDrive%
   if .%FichPath% == . set FichPath=%WDPath%
	
   set DirDestino=%FichDrive%%FichPath%
   
   if .%FichExtension% == . (
     set FichDestino=%DirDestino%%FichNombre%.sql
   ) else (
     set FichDestino=%DirDestino%%FichNombre%%FichExtension%
   )

   if NOT EXIST %DirDestino% (md %DirDestino% >nul)

:Fin_CheckFichero
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
:: NombreBD: Consulta el nombre que le corresponde a la BD
:: ---------------------------------------------------------------------------
:NombreBD
  set BD=%~1
  :: Obtener el nombre que le corresponde a la BD de edición
  mysql --defaults-file=%InfoConexion% -h %Servidor% -f -n -N -B -e "SELECT new_cd40_offline.GetNombreConf('%BD%')" > %SQLTempFile% 2> %ErrFile%
  if "%ERRORLEVEL%" NEQ "0" (
    echo.
    echo.    NombreBD_Err: Error al ejecutar la sentencia para consultar la BD de edición ^(_trans^) de %BD% >&2
    type %ErrFile% >>&2
    set HayError=S
	goto :Fin_NombreBD
  )

  :: El log incluirá el nombre que le corresponde a la BD de edicion
  set /a NumBD=0
  for /F "tokens=*" %%L in (%SQLTempFile%) do (
     set /a NumBD+=1
	 set BD=%%L
  )

  if %NumBD% NEQ 1 (
    echo.
    echo.    NombreBD_Err: Error al consultar la BD _trans de %BD% >&2
    set HayError=S
	goto :Fin_NombreBD
  )

:Fin_NombreBD
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
echo.     3. BD a exportar >>&2
echo.     4. Modo export: DATOS, ESQUEMA, TABLAS, CODIGO, TRIGGERS, TODO (por defecto) >>&2
echo.     5. Fichero destino del export >>&2
echo.     6. Servidor donde se encuentra la BD  >>&2

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
echo.    (%date% %time%)   Export finalizado
echo.
echo.
if EXIST %SQLTempFile% (del %SQLTempFile% >nul)
if EXIST %InfoConexion% (del %InfoConexion% >nul)
if EXIST %ErrFile% (del %ErrFile% >nul)

:FIN
ENDLOCAL
EXIT /B 0
