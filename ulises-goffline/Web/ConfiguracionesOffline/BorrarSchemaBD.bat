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
set TituloProceso=Borrar Schema BD SCV %date% %time%
title %TituloProceso%
set "InicioScript=%time: =0%"
REM ----------------------------------------------------------------------------
REM    Parámetros del script:
REM     1. GUID del proceso
REM     2. Usuario de la BD
REM     3. Password del usuario
REM     4. Nombre del schema de BD a borrar
REM     5. Servidor de BD
REM ----------------------------------------------------------------------------
for /F "tokens=*" %%L in ('hostname') do set Servidor=%%L
for /F "tokens=*" %%L in ('cd') do set DirTrabajo=%%L
if /I "%ProcesoPPAL%" == "" (set ProcesoPPAL=%0)
set ProcesoActual=%0
set Codigo_Error=ERR_DEL
set ArgumentosScript=%*
set ArgumentosScript=%ArgumentosScript:"=\"%

:PARAMETROS
if [%1] == [] goto :FIN_ERROR_PARAMETROS
if [%2] == [] goto :FIN_ERROR_PARAMETROS
if [%3] == [] goto :FIN_ERROR_PARAMETROS
if [%4] == [] goto :FIN_ERROR_PARAMETROS

Set GUID=%1
Set Usuario=%2
Set Password=%3
Set BD_Borrar=%4
if NOT [%5] == [] set Servidor=%5

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
call :LogScriptPPAL "  SCRIPT PARA BORRAR SCHEMA DE BD SCV"
call :LogScriptPPAL "__________________________________________________________________________"
call :LogScript ""

call :LogScript ""
call :LogScript "   BD_Borrar  : %BD_Borrar%"
call :LogScript "      Servidor   : %Servidor%"
if NOT [%ServidorLogProcesos%] == [%Servidor%] (
  call :LogScript "      ServidorLog: %ServidorLogProcesos%"
)
call :LogScript ""

call :AddLogProceso "Eliminando esquema %BD_Borrar% ..." || goto :FIN_ERROR

call :UpdLogProceso "Comprobando conf. %BD_Borrar%" || goto :FIN_ERROR
call :CheckBD_Existe %BD_Borrar% || goto :FIN_ERROR
if "!ExisteBD!" == "" goto :FIN_OK
if "!ExisteBD!" == "NO" goto :FIN_OK

:BORRAR_CONF
call :UpdLogProceso "Eliminando schema de BD %BD_Borrar%" || goto :FIN_ERROR
mysql --defaults-file=%InfoConexion% -h %Servidor% -f -n -N -B -e "DROP DATABASE %BD_Borrar%" > %SQLTempFile% 2> %ErrFile%
call :ComprobarErrores "Eliminando schema de BD %BD_Borrar%" "%ErrFile%" || goto :FIN_ERROR

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
REM CheckBD_Existe: Comprueba que la BD existe
REM ---------------------------------------------------------------------------
:CheckBD_Existe
  set BD=%~1
  REM Consultamos si la BD existe
  mysql --defaults-file=%InfoConexion% -h %Servidor% -f -n -N -B -e "SELECT SCHEMA_NAME FROM INFORMATION_SCHEMA.SCHEMATA WHERE SCHEMA_NAME = '%BD%'" > %SQLTempFile% 2> %ErrFile%
  call :ComprobarErrores "Consultando la BD %BD% en la lista de esquemas en %Servidor%" "%ErrFile%" || goto :FinErr_CheckBD_Existe

  REM El log debe tener 1 líneas con el nombre de la BD
  set ExisteBD=NO
  for /F "tokens=*" %%L in (%SQLTempFile%) do (
     set ExisteBD=SI
  )

:Fin_CheckBD_Existe
  goto :eof
:FinErr_CheckBD_Existe
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
call :LogScript "      3. Password del usuario root de la BD"
call :LogScript "      4. Nombre del schema de BD a borrar"
call :LogScript "      5. Servidor donde se encuentra la BD"
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
call :LogScriptPPAL "  ESQUEMA BORRADO ^(!Duracion!^)"
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
