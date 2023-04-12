@echo off
REM Tratamiento de errores:
REM https://superuser.com/questions/80485/exit-batch-file-from-subroutine
REM Se redirecciona la salida de las llamadas a subrutinas (call) en caso de error 
REM    CALL :SUBRUTINA || GOTO ETIQUETA_ERROR 
REM    ... equivale a ... 
REM    CALL :SUBRUTINA
REM    IF ERRORLEVEL 1 GOTO ETIQUETA_ERROR
REM SETLOCAL EnableDelayedExpansion permite evaluar las variables en ejecución
REM Por ejemplo, cuando se anidad if for y calls ...
SETLOCAL EnableDelayedExpansion
set TituloProceso=Exportar Conf. SCV %date% %time%
title %TituloProceso%
REM ----------------------------------------------------------------------------
REM    Parámetros del script:
REM     1. GUID del proceso
REM     2. Usuario de la BD
REM     3. Password del usuario
REM     4. BD a exportar 
REM        Será el nombre de la BD con la conf principal.
REM        El script exportará tanto esa BD como la BD de edición (_trans)
REM     5. Versión del export
REM     6. Modo export: Indicará el tipo de export. Los posibles valores son:
REM        DATOS   : Solo exporta los datos
REM        ESQUEMA : Solo exporta los objetos de la BD sin los datos de las tablas
REM        TABLAS  : Solo exporta las tablas y las vistas
REM        CODIGO  : Solo exporta el código de la BD, triggers y rutinas
REM        TRIGGERS: Solo exporta los triggers de la BD
REM        TODO    : hace un export completo (por defecto)
REM     7. Fichero destino del export
REM     8. Servidor donde se encuentra la BD
REM ----------------------------------------------------------------------------
for /F "tokens=*" %%L in ('hostname') do set Servidor=%%L
for /F "tokens=*" %%L in ('cd') do set DirTrabajo=%%L
if /I "%ProcesoPPAL%" == "" (set ProcesoPPAL=%0)
set Codigo_Error=ERR_EXP
set ProcesoActual=%0
set ArgumentosScript=%*
set ArgumentosScript=%ArgumentosScript:"=\"%
set "InicioScript=%time: =0%"

:PARAMETROS
if [%1] == [] goto :FIN_ERROR_PARAMETROS
if [%2] == [] goto :FIN_ERROR_PARAMETROS
if [%3] == [] goto :FIN_ERROR_PARAMETROS
if [%4] == [] goto :FIN_ERROR_PARAMETROS
if [%5] == [] goto :FIN_ERROR_PARAMETROS
	
Set GUID=%1
Set Usuario=%2
Set Password=%3
Set BD_CD40=%4
Set Version_Export=%~5
if /I [%6] == [DATOS] (
  set TipoExport=DATOS
) else (
  if /I [%6] == [ESQUEMA] (
    set TipoExport=ESQUEMA
  ) else (
    if /I [%6] == [TABLAS] (
      set TipoExport=TABLAS
    ) else (
      if /I [%6] == [CODIGO] (
        set TipoExport=CODIGO
      ) else (
        if /I [%6] == [TRIGGERS] (
          set TipoExport=TRIGGERS
        ) else (
          set TipoExport=TODO
        )
      )
    )
  )
)

if NOT [%8] == [] set Servidor=%8

REM Configuración de la conexión a la BD y ficheros temporales del script
call :ConfConexion

if NOT [%Servidor_Origen%] == [] (
  set ServidorLogProcesos=%Servidor_Origen%
) else (
  set ServidorLogProcesos=%Servidor%
)

:INICIO
call :LogScriptPPAL "__________________________________________________________________________"
call :LogScriptPPAL ""
call :LogScriptPPAL "  SCRIPT PARA EXPORTAR UNA CONF. SCV"
call :LogScriptPPAL "__________________________________________________________________________"
call :LogScript ""

REM PID del proceso
REM En lugar de usar el PID para registrar el log del proceso se pasará a usar el GUID que se pasa como primer parámetro del script
REM tasklist /fi "imagename eq cmd.exe" /v  > %LogTempFile%
REM for /F "tokens=1,2,*" %%a in ('findstr /irc:".*%TituloProceso%" %LogTempFile%') do  set PIDComando=%%b

call :LogScript "   Exportar %BD_CD40%:"
call :LogScript "      Usuario      : %Usuario%"
call :LogScript "      Export       : %TipoExport%"
call :LogScript "      Servidor     : %Servidor%"
if NOT [%ServidorLogProcesos%] == [%Servidor%] (
  call :LogScript "      ServidorLog  : %ServidorLogProcesos%"
)

call :AddLogProceso "Exportando %BD_CD40% ..." || goto :FIN_ERROR

call :UpdLogProceso "Comprobando conf. %BD_CD40%" || goto :FIN_ERROR
call :NombreBD %BD_CD40% || goto :FIN_ERROR
set BD_CD40=%BD%

call :CheckBD_Existe %BD_CD40% || goto :FIN_ERROR

REM Comprobamos el nombre del fichero y el directorio
REM Lo hacemos después de comprobar la BD para poder 
REM darle un nombre por defecto al fichero de salida
call :CheckFichero %7 || goto :FIN_ERROR
call :LogScript "      Fichero      : %FichDestino%"

call :NombreBD_Ed %BD_CD40% || goto :FIN_ERROR
set BD_CD40_Ed=%BD_Ed%

call :CheckBD_Existe %BD_CD40_Ed% || goto :FIN_ERROR

call :LogScript ""   
call :LogScript "      BD_CD40      : %BD_CD40%"
call :LogScript "      BD_CD40_Ed   : %BD_CD40_Ed%"

:EXPORT
REM Exportamos sólo los datos
if /I "%TipoExport%" == "DATOS" (
  call :UpdLogProceso "Exportando DATOS de %BD_CD40%" || goto :FIN_ERROR
  mysqldump --defaults-file=%InfoConexion% -h %Servidor% -B --skip-triggers -t --databases %BD_CD40% %BD_CD40_Ed% > "%FichDestino%" 2> %ErrFile%
  call :ComprobarErrores "" "%ErrFile%" || goto :FIN_ERROR
  goto :FIN_EXPORT
)

REM Exportamos sólo el esquema
if /I "%TipoExport%" == "ESQUEMA" (
  call :UpdLogProceso "Exportando ESQUEMA de %BD_CD40%" || goto :FIN_ERROR
  mysqldump --defaults-file=%InfoConexion% -h %Servidor% -B --no-data -R --databases %BD_CD40% %BD_CD40_Ed% > "%FichDestino%" 2> %ErrFile%
  call :ComprobarErrores "" "%ErrFile%" || goto :FIN_ERROR
  goto :FIN_EXPORT
)

REM Exportamos sólo las tablas
if /I "%TipoExport%" == "TABLAS" (
  call :UpdLogProceso "Exportando TABLAS de %BD_CD40%" || goto :FIN_ERROR
  mysqldump --defaults-file=%InfoConexion% -h %Servidor% -B --no-data --skip-triggers --databases %BD_CD40% %BD_CD40_Ed% > "%FichDestino%" 2> %ErrFile%
  call :ComprobarErrores "" "%ErrFile%" || goto :FIN_ERROR
  goto :FIN_EXPORT
)

REM Exportamos sólo el código
if /I "%TipoExport%" == "CODIGO" (
  call :UpdLogProceso "Exportando CODIGO de %BD_CD40%" || goto :FIN_ERROR
  mysqldump --defaults-file=%InfoConexion% -h %Servidor% -B --no-data --no-create-db --no-create-info --routines --add-drop-trigger --triggers --skip-opt --lock-tables --databases %BD_CD40% %BD_CD40_Ed% > "%FichDestino%" 2> %ErrFile%
  call :ComprobarErrores "" "%ErrFile%" || goto :FIN_ERROR
  goto :FIN_EXPORT
)

REM Exportamos sólo los triggers
if /I "%TipoExport%" == "TRIGGERS" (
  call :UpdLogProceso "Exportando TRIGGERS de %BD_CD40%" || goto :FIN_ERROR
  mysqldump --defaults-file=%InfoConexion% -h %Servidor% -B --no-data --no-create-db --no-create-info --add-drop-trigger --triggers --skip-opt --lock-tables --databases %BD_CD40% %BD_CD40_Ed% > "%FichDestino%" 2> %ErrFile%
  call :ComprobarErrores "" "%ErrFile%" || goto :FIN_ERROR
  goto :FIN_EXPORT
)

REM Exportamos TODA la BD
if /I "%TipoExport%" == "TODO" (
  call :UpdLogProceso "Exportando TODO de %BD_CD40%" || goto :FIN_ERROR
  mysqldump --defaults-file=%InfoConexion% -h %Servidor% -B -R --databases %BD_CD40% %BD_CD40_Ed%  > "%FichDestino%" 2> %ErrFile%
  call :ComprobarErrores "" "%ErrFile%" || goto :FIN_ERROR
  goto :FIN_EXPORT
)

:FIN_EXPORT
REM Convertimos el juego de caracteres a utf8
call :UpdLogProceso "Cambiando juego de caracteres a utf8" || goto :FIN_ERROR
powershell -ExecutionPolicy Bypass -File .\CambiarCodificacion.ps1 "%FichDestino%" "UTF8" 2> %ErrFile%
call :ComprobarErrores "" "%ErrFile%" || goto :FIN_ERROR

REM Quitar el nombre de la BD Origen de los triggers en el fichero de salida
call :UpdLogProceso "Quitando nombre de la BD de los triggers" || goto :FIN_ERROR
powershell -Command "(gc %FichDestino%) -replace '`%BD_CD40_Ed%`\.', '' | Out-File %FichDestino%" 2> %ErrFile%
call :ComprobarErrores "" "%ErrFile%" || goto :FIN_ERROR

powershell -Command "(gc %FichDestino%) -replace '%BD_CD40_Ed%\.', ''   | Out-File %FichDestino%" 2> %ErrFile%
call :ComprobarErrores "" "%ErrFile%" || goto :FIN_ERROR

powershell -Command "(gc %FichDestino%) -replace '`%BD_CD40%`\.', ''    | Out-File %FichDestino%" 2> %ErrFile%
call :ComprobarErrores "" "%ErrFile%" || goto :FIN_ERROR

powershell -Command "(gc %FichDestino%) -replace '%BD_CD40%\.', ''      | Out-File %FichDestino%" 2> %ErrFile%
call :ComprobarErrores "" "%ErrFile%" || goto :FIN_ERROR


REM Convertimos el juego de caracteres a utf8 sin BOM
call :UpdLogProceso "Cambiando juego de caracteres a utf8 sin BOM" || goto :FIN_ERROR
powershell -ExecutionPolicy Bypass -File .\CambiarCodificacion.ps1 %FichDestino% "Utf8SinBOM" 2> %ErrFile%
call :ComprobarErrores "" "%ErrFile%" || goto :FIN_ERROR

REM Añadir la versión al final del fichero
echo.%Version_Export% >> %FichDestino%
powershell -ExecutionPolicy Bypass -File .\CambiarCodificacion.ps1 %FichDestino% "Utf8SinBOM" 2> %ErrFile%

call :EndLogProceso "Proceso finalizado"
goto :FIN_OK


REM ---------------------------------------------------------------------------
REM
REM            P R O C E D I M I E N T O S   D E L   S C R I P T 
REM
REM ---------------------------------------------------------------------------
REM ConfConexion
REM ---------------------------------------------------------------------------
:ConfConexion
   set InfoConexion=tmpConn_%~n0.cnf
   echo.[client]             > %InfoConexion%
   echo.user=%Usuario%      >> %InfoConexion%
   echo.password=%Password% >> %InfoConexion%
   set DumpTempFile=tmp_dump_%~n0.sql
   set SQLTempFile=tmp_%~n0.sql
   set LogTempFile=tmp_%~n0.log
   set ErrFile=tmp_%~n0.err
   set ErrLogProceso=tmp_LogProceso.err

:Fin_ConfConexion
  goto :eof

REM ---------------------------------------------------------------------------
REM ComprobarErrores: comprueba los errores en el fichero que se le pasa como parámetro
REM ---------------------------------------------------------------------------
:ComprobarErrores
  set procedimiento=%~1
  set ficheroErrores=%~2

  if "%ficheroErrores%" NEQ "" goto :ComprobarErrores_ConLogProceso
  if /I "%ficheroErrores%" == "%ErrLogProceso%" goto :ComprobarErrores_SinLogProceso

:ComprobarErrores_ErrorLevel
  if ERRORLEVEL 1 (
     call :LogError "%procedimiento%"
     call :EndLogProceso "!errMsg!"
     EXIT /B 1
  )
  goto :FinComprobarErrores
  
:ComprobarErrores_ConLogProceso
  for /F "tokens=*" %%L in (%ficheroErrores%) do (
     call :LogError "%procedimiento%" "%ficheroErrores%"
     call :EndLogProceso "!errMsg!"
     EXIT /B 1
  )
  goto :FinComprobarErrores
  
:ComprobarErrores_SinLogProceso
  for /F "tokens=*" %%L in (%ficheroErrores%) do (
     call :LogError "%procedimiento%" "%ficheroErrores%"
     EXIT /B 1
  )
  goto :FinComprobarErrores

:FinComprobarErrores
  goto :eof

REM ---------------------------------------------------------------------------
REM LogScriptPPAL: Se deja log si el proceso es el ppal
REM ---------------------------------------------------------------------------
:LogScriptPPAL
  if "%ProcesoPPAL%" NEQ "%ProcesoActual%" goto FinLogScriptPPAL
  echo.%~1

:FinLogScriptPPAL
  goto :eof

REM ---------------------------------------------------------------------------
REM LogScript: Se deja log del proceso
REM ---------------------------------------------------------------------------
:LogScript
  echo.%~1
  goto :eof

REM ---------------------------------------------------------------------------
REM LogError: procesa los errores
REM ---------------------------------------------------------------------------
:LogError
  set errMsg=%~1
  set errFich=%~2
  
:LogErrorMsg
  if "%errMsg%" == "" (
     set errMsg=%Codigo_Error%
     goto :LogErrorFich
  )
  
  set errMsg=%Codigo_Error%: %errMsg%
  echo.%errMsg%>&2

:LogErrorFich
  if "%errFich%" == "" goto :FinLogError

  set /a linea=0
  for /F "tokens=*" %%A in (%errFich%) do (
     set /a linea+=1
	 if "!linea!" == "1" (
        for /F "tokens=1,* delims=:" %%L in ("%%A") do (
		   set codigo=%%L
		   if /I "!codigo:~0,4!" == "ERR_" (
              set errMsg=%errMsg%. %%A
              echo.%%A >>&2
		   ) else (
              REM set errMsg=%errMsg%. %%M REM Ponemos todo el mensaje del error, no quitamos lo que hay antes de los dos puntos
              REM echo.%%M >>&2
			  set errMsg=%errMsg%. %%A
              echo.%%A >>&2
		   )
        )
     ) else (
        set errMsg=%errMsg%. %%A
        echo.%%A >>&2
     )	
  )

:FinLogError
  goto :eof

REM ---------------------------------------------------------------------------
REM AddLogProceso: deja log de la ejecución del proceso en la tabla LogProcesos
REM ---------------------------------------------------------------------------
:AddLogProceso
   if "%ProcesoPPAL%" NEQ "%ProcesoActual%" goto :AddLogProceso_SubProceso
   powershell -ExecutionPolicy Bypass -File .\LogProceso.ps1 -UsuarioBD "%Usuario%" -PasswordBD "%Password%" -ServidorBD "%ServidorLogProcesos%" -GUID "%GUID%" -Proceso "%ProcesoActual%" -Argumentos "%ArgumentosScript%" -Descripcion "%~1" -Activo 1 -ReplaceDesc 2> %ErrLogProceso%
   call :ComprobarErrores "Insertando el log del proceso" "%ErrLogProceso%" || goto :FinErrAddLogProceso

:AddLogProceso_SubProceso
   powershell -ExecutionPolicy Bypass -File .\LogProceso.ps1 -UsuarioBD "%Usuario%" -PasswordBD "%Password%" -ServidorBD "%ServidorLogProcesos%" -GUID "%GUID%" -Proceso "%ProcesoActual%" -Argumentos "%ArgumentosScript%" -Descripcion "%~1" -Activo 1 2> %ErrLogProceso%
   call :ComprobarErrores "Insertando el log del proceso" "%ErrLogProceso%" || goto :FinErrAddLogProceso

:FinAddLogProceso
  goto :eof
:FinErrAddLogProceso
  EXIT /B 1

REM ---------------------------------------------------------------------------
REM UpdLogProceso: Modifica la descripción del proceso
REM ---------------------------------------------------------------------------
:UpdLogProceso
   if "%ProcesoPPAL%" NEQ "%ProcesoActual%" goto :FinUpdLogProceso
   powershell -ExecutionPolicy Bypass -File .\LogProceso.ps1 -UsuarioBD "%Usuario%" -PasswordBD "%Password%" -ServidorBD "%ServidorLogProcesos%" -GUID "%GUID%" -Descripcion "%~1" 2> %ErrLogProceso%
   call :ComprobarErrores "Modificando el log del proceso" "%ErrLogProceso%" || goto :FinErrUpdLogProceso

:FinUpdLogProceso
  goto :eof
:FinErrUpdLogProceso
  EXIT /B 1

REM ---------------------------------------------------------------------------
REM EndLogProceso: da por finalizado el proceso (activo = 0)
REM ---------------------------------------------------------------------------
:EndLogProceso
   if "%ProcesoPPAL%" NEQ "%ProcesoActual%" goto :FinEndLogProceso
   powershell -ExecutionPolicy Bypass -File .\LogProceso.ps1 -UsuarioBD "%Usuario%" -PasswordBD "%Password%" -ServidorBD "%ServidorLogProcesos%" -GUID "%GUID%" -Descripcion "%~1" -Activo "0" 2> %ErrLogProceso%
   call :ComprobarErrores "Marcando el proceso como finalizado" "%ErrLogProceso%" || goto :FinErrEndLogProceso

   REM Registrar el proceso en HistoricoIncidencias
   powershell -ExecutionPolicy Bypass -File .\NuevaIncidenciaGOffline.ps1 -UsuarioBD "%Usuario%" -PasswordBD "%Password%" -ServidorBD "%ServidorLogProcesos%" -IdLogProceso "%GUID%" 2> %ErrLogProceso%
   call :ComprobarErrores "Registrando proceso en HistoricoIncidencias" "%ErrLogProceso%" || goto :FinErrEndLogProceso

   goto :FinEndLogProceso
   
REM :EndLogProceso_Activo
REM    powershell -ExecutionPolicy Bypass -File .\LogProceso.ps1 -UsuarioBD "%Usuario%" -PasswordBD "%Password%" -ServidorBD "%ServidorLogProcesos%" -GUID "%GUID%" -Descripcion "%~1" 2> %ErrLogProceso%
REM    call :ComprobarErrores "Finalizando proceso" "%ErrLogProceso%" || goto :FinErrEndLogProceso

:FinEndLogProceso
  goto :eof
:FinErrEndLogProceso
  EXIT /B 1

REM ---------------------------------------------------------------------------
REM DuracionScript: calcula la %Duracion% del script
REM ---------------------------------------------------------------------------
:DuracionScript
  set "FinScript=%time: =0%"

  REM Duración del script
  set "Fin=!FinScript:%time:~8,1%=%%100)*100+1!"  &  set "Inicio=!InicioScript:%time:~8,1%=%%100)*100+1!"
  set /A "dur=((((10!Fin:%time:~2,1%=%%100)*60+1!%%100)-((((10!Inicio:%time:~2,1%=%%100)*60+1!%%100), elap-=(elap>>31)*24*60*60*100"

  REM Convertir duración al formato HH:MM:SS:CC:
  set /A "cc=dur%%100+100,dur/=100,ss=dur%%60+100,dur/=60,mm=dur%%60+100,hh=dur/60+100"
  set "Duracion=%hh:~1%%time:~2,1%%mm:~1%%time:~2,1%%ss:~1%%time:~8,1%%cc:~1%"

:FinDuracionScript
  goto :eof
  

REM ---------------------------------------------------------------------------
REM CheckFichero
REM ---------------------------------------------------------------------------
:CheckFichero
   set Fich=%~f1

   if "%Fich%" == "" set Fich=%DirTrabajo%\%BD_CD40%.sql

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

   if "%FichDrive%" == "" set FichDrive=%WDDrive%
   if "%FichPath%" == "" set FichPath=%WDPath%
	
   set DirDestino=%FichDrive%%FichPath%
   
   if "%FichExtension%" == "" (
     set FichDestino=%DirDestino%%FichNombre%.sql
   ) else (
     set FichDestino=%DirDestino%%FichNombre%%FichExtension%
   )

   if NOT EXIST %DirDestino% (md %DirDestino% >nul)

:Fin_CheckFichero
  goto :eof

REM ---------------------------------------------------------------------------
REM CheckBD_Existe: Comprueba que la BD existe
REM ---------------------------------------------------------------------------
:CheckBD_Existe
  set BD=%~1
  REM Consultamos si la BD / Configuración está en la lista de configuraciones
  mysql --defaults-file=%InfoConexion% -h %Servidor% -f -n -N -B -e "SELECT NOMBRE FROM new_cd40_offline.CONFIGURACIONES WHERE NOMBRE = '%BD%' OR CONF_EDICION = '%BD%'" > %SQLTempFile% 2> %ErrFile%
  call :ComprobarErrores "Consultando la BD %BD% en la lista de CONFIGURACIONES" "%ErrFile%" || goto :FinErr_CheckBD_Existe

  REM El log debe tener 1 líneas con el nombre de la BD
  set /a NumBD=0
  for /F "tokens=*" %%L in (%SQLTempFile%) do (
     set /a NumBD+=1
  )

  REM ¿Debería fallar si la BD no está en la lista de configuraciones?
  REM if %NumBD% NEQ 1 (
  REM   call :LogError "La BD %BD% no está en la lista de CONFIGURACIONES"
  REM )

  REM Consultamos si la BD / Configuración existe como BD
  mysql --defaults-file=%InfoConexion% -h %Servidor% -f -n -N -B -e "SELECT SCHEMA_NAME FROM INFORMATION_SCHEMA.SCHEMATA WHERE SCHEMA_NAME = '%BD%'" > %SQLTempFile% 2> %ErrFile%
  call :ComprobarErrores "Consultando la BD %BD% en la lista de esquemas del servidor ^(INFORMATION_SCHEMA.SCHEMATA^)" "%ErrFile%" || goto :FinErr_CheckBD_Existe

  REM El log debe tener 1 líneas con el nombre de la BD
  set /a NumBD=0
  for /F "tokens=*" %%L in (%SQLTempFile%) do (
     set /a NumBD+=1
  )

  if %NumBD% NEQ 1 (
    call :LogError "La BD %BD% no existe"
	goto :FinErr_CheckBD_Existe 
  )

:Fin_CheckBD_Existe
  goto :eof
:FinErr_CheckBD_Existe 
  EXIT /B 1

REM ---------------------------------------------------------------------------
REM NombreBD: Consulta el nombre que le corresponde a la BD
REM ---------------------------------------------------------------------------
:NombreBD
  set BD=%~1
  REM Obtener el nombre que le corresponde a la BD de edición
  mysql --defaults-file=%InfoConexion% -h %Servidor% -f -n -N -B -e "SELECT new_cd40_offline.GetNombreConf('%BD%')" > %SQLTempFile% 2> %ErrFile%
  call :ComprobarErrores "Consultando la BD %BD%" "%ErrFile%" || goto :FinErr_NombreBD

  REM El log incluirá el nombre que le corresponde a la BD de edicion
  set /a NumBD=0
  for /F "tokens=*" %%L in (%SQLTempFile%) do (
     set /a NumBD+=1
	 set BD=%%L
  )

  if %NumBD% NEQ 1 (
    call :LogError "Error al consultar la BD _trans de %BD%"
	goto :FinErr_NombreBD
  )

:Fin_NombreBD
  goto :eof
:FinErr_NombreBD
  EXIT /B 1

REM ---------------------------------------------------------------------------
REM NombreBD_Ed: Consulta el nombre que le corresponde a la BD de Edición
REM ---------------------------------------------------------------------------
:NombreBD_Ed
  set BD=%~1
  REM Obtener el nombre que le corresponde a la BD de edición
  mysql --defaults-file=%InfoConexion% -h %Servidor% -f -n -N -B -e "SELECT new_cd40_offline.GetNombreConfEdicion('%BD%')" > %SQLTempFile% 2> %ErrFile%
  call :ComprobarErrores "Consultando la BD de ed. ^(_trans^) de %BD%" "%ErrFile%" || goto :FinErr_NombreBD_Ed

  REM El log incluirá el nombre que le corresponde a la BD de edicion
  set /a NumBD=0
  for /F "tokens=*" %%L in (%SQLTempFile%) do (
     set /a NumBD+=1
	 set BD_Ed=%%L
  )

  if %NumBD% NEQ 1 (
    call :LogError "Error al consultar la BD _trans de %BD%"
	goto :FinErr_NombreBD_Ed
  )

:Fin_NombreBD_Ed
  goto :eof
:FinErr_NombreBD_Ed
  EXIT /B 1


REM ---------------------------------------------------------------------------
REM
REM            F I N  D E L   S C R I P T 
REM
REM ---------------------------------------------------------------------------
:FIN_ERROR_PARAMETROS
call :LogScriptPPAL ""
call :LogScriptPPAL "    (%date% %time%)
call :LogScriptPPAL ""
call :LogScript "    ERROR PARAMETROS"
call :LogScript "      1. GUID del proceso"
call :LogScript "      2. Usuario de la BD"
call :LogScript "      3. Password del usuario"
call :LogScript "      4. BD a exportar"
call :LogScript "      5. Versión
call :LogScript "      6. Modo export: DATOS, ESQUEMA, TABLAS, CODIGO, TRIGGERS, TODO ^(por defecto^)"
call :LogScript "      7. Fichero destino del export"
call :LogScript "      8. Servidor donde se encuentra la BD"
call :LogError "ERROR PARAMETROS"

:FIN_ERROR
call :DuracionScript
call :LogScriptPPAL ""
call :LogScriptPPAL " ________________________________________________________________________"
call :LogScriptPPAL ""
call :LogScriptPPAL "    SE HA PRODUCIDO UN ERROR ^(!Duracion!^)"
call :LogScriptPPAL " ________________________________________________________________________"
call :LogScriptPPAL "
ENDLOCAL
EXIT /B 1

:FIN_OK
call :DuracionScript
call :LogScriptPPAL " ________________________________________________________________________"
call :LogScriptPPAL ""
call :LogScriptPPAL "  EXPORT FINALIZADO ^(!Duracion!^)"
call :LogScriptPPAL " ________________________________________________________________________"
call :LogScriptPPAL "
if EXIST %InfoConexion% (del %InfoConexion% >nul)
if EXIST %DumpTempFile% (del %DumpTempFile% >nul)
if EXIST %SQLTempFile% (del %SQLTempFile% >nul)
if EXIST %LogTempFile% (del %LogTempFile% >nul)
if EXIST %ErrFile% (del %ErrFile% >nul)
if EXIST %ErrLogProceso% (del %ErrLogProceso% >nul)

:FIN
ENDLOCAL
EXIT /B 0
