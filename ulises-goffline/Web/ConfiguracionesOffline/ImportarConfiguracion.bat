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
set TituloProceso=Importar Conf. SCV %date% %time%
title %TituloProceso%
REM ----------------------------------------------------------------------------
REM    Parámetros del script:
REM     1. GUID del proceso
REM     2. Usuario de la BD
REM     3. Password del usuario
REM     4. BD sobre la que importar 
REM        Será el nombre de la BD con la conf principal.
REM        El script importará tanto esa BD como la BD de edición (_trans)
REM     5. Descripción de la BD
REM     6. Borrar: 0/1. Indica si hay que borrar la BD en caso de que exista. Por defecto 0
REM     7. Fichero origen del import
REM     8. Servidor donde se importará la BD
REM ----------------------------------------------------------------------------
for /F "tokens=*" %%L in ('hostname') do set Servidor=%%L
for /F "tokens=*" %%L in ('cd') do set DirTrabajo=%%L
if /I "%ProcesoPPAL%" == "" (set ProcesoPPAL=%0)
set Codigo_Error=ERR_IMP
set ProcesoActual=%0
set ArgumentosScript=%*
set ArgumentosScript=%ArgumentosScript:"=\"%
set "InicioScript=%time: =0%"

:PARAMETROS
if [%1] == [] goto :FIN_ERROR_PARAMETROS
if [%2] == [] goto :FIN_ERROR_PARAMETROS
if [%3] == [] goto :FIN_ERROR_PARAMETROS
if [%4] == [] goto :FIN_ERROR_PARAMETROS

Set GUID=%1
Set Usuario=%2
Set Password=%3
Set BD_CD40_Destino=%4
Set BD_CD40_Destino_Desc=%~5
if [%6] == [1] (
  set BorrarBD=1
) else (
  set BorrarBD=0
)	
if [%7] == [] set FichOrigen=%DirTrabajo%\%BD_CD40_Destino%.sql
if NOT [%7] == [] set FichOrigen=%7
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
call :LogScriptPPAL "  SCRIPT PARA IMPORTAR LA CONF. SCV"
call :LogScriptPPAL "__________________________________________________________________________"
call :LogScript ""

REM PID del proceso
REM En lugar de usar el PID para registrar el log del proceso se pasará a usar el GUID que se pasa como primer parámetro del script
REM tasklist /fi "imagename eq cmd.exe" /v  > %LogTempFile%
REM for /F "tokens=1,2,*" %%a in ('findstr /irc:".*%TituloProceso%" %LogTempFile%') do  set PIDComando=%%b

call :LogScript "   Importar %FichOrigen% en %BD_CD40_Destino%:"
call :LogScript "      Usuario    : %Usuario%"
call :LogScript "      Servidor   : %Servidor%"
if NOT [%ServidorLogProcesos%] == [%Servidor%] (
  call :LogScript "      ServidorLog: %ServidorLogProcesos%"
)
if "%BorrarBD%" == "1" (
  call :LogScript "      BorrarBD   : SI"
) else (
  call :LogScript "      BorrarBD   : NO"
)	

if "%ProcesoPPAL%" == "%ProcesoActual%" (
  call :AddLogProceso "Importando %FichOrigen% ..." || goto :FIN_ERROR
) else (
  call :AddLogProceso "Importando %BD_CD40_Destino% en %Servidor% ..." || goto :FIN_ERROR
)

if NOT EXIST %FichOrigen% (
  call :LogError "El fichero %FichOrigen% no existe"
  
)

call :UpdLogProceso "Comprobando origen de datos" || goto :FIN_ERROR
call :NombreBD_Origen %FichOrigen% || goto :FIN_ERROR
set BD_CD40_Origen=%BD%
set BD_CD40_Origen_Ed=%BD_Ed%

call :LogScript ""   
call :LogScript "      BD_CD40_Origen    : %BD_CD40_Origen%"
call :LogScript "      BD_CD40_Origen_Ed : %BD_CD40_Origen_Ed%"

call :UpdLogProceso "Comprobando BD destino de datos" || goto :FIN_ERROR
call :CheckNombreBD %BD_CD40_Destino% || goto :FIN_ERROR
set BD_CD40_Destino=%BD%

call :NombreBD_Ed %BD_CD40_Destino% || goto :FIN_ERROR
set BD_CD40_Destino_Ed=%BD_Ed%

call :LogScript ""   
call :LogScript "      BD_CD40_Destino   : %BD_CD40_Destino%"
call :LogScript "      BD_CD40_Destino_Ed: %BD_CD40_Destino_Ed%"
call :LogScript ""

type %FichOrigen% > %DumpTempFile%

REM ----------------------------------------------------------------------------
REM Cambiamos en el fichero el nombre de la BD Origen por el de la BD Destino
REM ----------------------------------------------------------------------------
REM Convertimos el juego de caracteres a utf8
call :UpdLogProceso "Cambiando la BD para importar" || goto :FIN_ERROR
powershell -ExecutionPolicy Bypass -File .\CambiarCodificacion.ps1 "%DumpTempFile%" "UTF8" 2> %ErrFile%
call :ComprobarErrores "" "%ErrFile%" || goto :FIN_ERROR

REM Cambiar el nombre de la BD Origen por el de la BD Destino
powershell -Command "(gc %DumpTempFile%) -replace '`%BD_CD40_Origen_Ed%`', '`%BD_CD40_Destino_Ed%`' | Out-File %DumpTempFile%" 2> %ErrFile%
call :ComprobarErrores "" "%ErrFile%" || goto :FIN_ERROR

REM Cambiar el nombre de la BD Origen por el de la BD Destino
powershell -Command "(gc %DumpTempFile%) -replace '`%BD_CD40_Origen%`', '`%BD_CD40_Destino%`' | Out-File %DumpTempFile%" 2> %ErrFile%
call :ComprobarErrores "" "%ErrFile%" || goto :FIN_ERROR

REM Convertimos el juego de caracteres a utf8 sin BOM
powershell -ExecutionPolicy Bypass -File .\CambiarCodificacion.ps1 "%DumpTempFile%" "Utf8SinBOM" 2> %ErrFile%
call :ComprobarErrores "" "%ErrFile%" || goto :FIN_ERROR

REM ----------------------------------------------------------------------------
REM Proceso de importación en las BD destino
REM ----------------------------------------------------------------------------
call :UpdLogProceso "Creando conf." || goto :FIN_ERROR
mysql --defaults-file=%InfoConexion% -h %Servidor% -f -n -N -B -e "CALL new_cd40_offline.AddConfiguracion('%BD_CD40_Destino%', '%BD_CD40_Destino_Desc%', %BorrarBD%)" > "%SQLTempFile%" 2> %ErrFile%
call :ComprobarErrores "Insertando conf. en la lista" "%ErrFile%" || goto :FIN_ERROR

REM Importamos las BD
call :UpdLogProceso "Importando las BD en %BD_CD40_Destino%" || goto :FIN_ERROR
mysql --defaults-file=%InfoConexion% -h %Servidor% -f -n -N -B -c -e "source "%DumpTempFile%";" 2> %ErrFile% 
call :ComprobarErrores "Importando en %BD_CD40_Destino% el fichero" "%ErrFile%" || goto :FIN_ERROR

REM Borrar Historicos
call :UpdLogProceso "Borrando Hist. Incidencias de %BD_CD40_Destino%" || goto :FIN_ERROR
mysql --defaults-file=%InfoConexion% -h %Servidor% -f -n -N -B -e "CALL new_cd40_offline.DelHistoricos('%BD_CD40_Destino%')" > "%SQLTempFile%" 2> %ErrFile%
call :ComprobarErrores "Borrando el histórico de incidencias de %BD_CD40_Destino%" "%ErrFile%" || goto :FIN_ERROR

REM Borrar Historicos
call :UpdLogProceso "Copiando Operadores de la instalación en %BD_CD40_Destino%" || goto :FIN_ERROR
mysql --defaults-file=%InfoConexion% -h %Servidor% -f -n -N -B -e "CALL new_cd40_offline.CopyTabla('%BD_CD40_Destino%', 'OPERADORES')" > %SQLTempFile% 2> %ErrFile%
call :ComprobarErrores "Copiando los operadores del servidor en la conf %BD_CD40_Destino%" "%ErrFile%" || goto :FIN_ERROR

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
REM NombreBD_Origen
REM ---------------------------------------------------------------------------
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
  REM Obtener el nombre que le corresponde a la BD origen
  mysql --defaults-file=%InfoConexion% -h %Servidor% -f -n -N -B -e "SELECT new_cd40_offline.GetNombreConf('%BD%')" > %SQLTempFile% 2> %ErrFile%
  call :ComprobarErrores "Consultando el nombre de la BD ^(%BD%^)" "%ErrFile%" || goto :FinErr_NombreBD_Origen
  
  REM El log incluirá el nombre que le corresponde a la BD
  set /a NumBD=0
  for /F "tokens=*" %%L in (%SQLTempFile%) do (
     set /a NumBD+=1
	 set BD=%%L
  )

  if %NumBD% NEQ 1 (
    call :LogError "Error al consultar el nombre de la BD %BD%"
	goto :FinErr_NombreBD_Origen
  )

  REM Obtener el nombre que le corresponde a la BD de edición
  mysql --defaults-file=%InfoConexion% -h %Servidor% -f -n -N -B -e "SELECT new_cd40_offline.GetNombreConfEdicion('%BD%')" > %SQLTempFile% 2> %ErrFile%
  call :ComprobarErrores "Consultando el nombre de la BD de ed. ^(_trans^) de %BD%" "%ErrFile%" || goto :FinErr_NombreBD_Origen

  REM El log incluirá el nombre que le corresponde a la BD
  set /a NumBD=0
  for /F "tokens=*" %%L in (%SQLTempFile%) do (
     set /a NumBD+=1
	 set BD_Ed=%%L
  )

  if %NumBD% NEQ 1 (
    call :LogError "Error al consultar la BD _trans de %BD%"
	goto :FinErr_NombreBD_Origen
  )

:Fin_NombreBD_Origen 
  goto :eof
:FinErr_NombreBD_Origen 
  EXIT /B 1

REM ---------------------------------------------------------------------------
REM CheckNombreBD
REM Comprobar que el nombre de la nueva conf cumple la norma
REM ---------------------------------------------------------------------------
:CheckNombreBD
  set BD=%~1
  REM Obtenemos el nombre que le corresponde a la BD/configuración, que cumpla la norma de new_cd40_*
  mysql --defaults-file=%InfoConexion% -h %Servidor% -f -n -N -B -e "SELECT new_cd40_offline.GetNombreConf('%BD%')" > %SQLTempFile% 2> %ErrFile%
  call :ComprobarErrores "Consultando el nombre que le corresponde a la conf %BD%" "%ErrFile%" || goto :FinErr_CheckNombreBD
  
  REM El log incluirá el nombre que le corresponde a la BD
  set /a NumBD=0
  for /F "tokens=*" %%L in (%SQLTempFile%) do (
     set /a NumBD+=1
	 set BD=%%L
  )

  if %NumBD% NEQ 1 (
    call :LogError "Error al consultar la BD de %BD%"
	goto :FinErr_CheckNombreBD
  )

:Fin_CheckNombreBD
  goto :eof
:FinErr_CheckNombreBD
  EXIT /B 1

REM ---------------------------------------------------------------------------
REM NombreBD_Ed: Consulta el nombre que le corresponde a la BD de Edición
REM ---------------------------------------------------------------------------
:NombreBD_Ed
  set BD=%~1
  REM Obtener el nombre que le corresponde a la BD de edición
  mysql --defaults-file=%InfoConexion% -h %Servidor% -f -n -N -B -e "SELECT new_cd40_offline.GetNombreConfEdicion('%BD%')" > %SQLTempFile% 2> %ErrFile%
  call :ComprobarErrores "Consultando la BD de edición ^(_trans^) de %BD%" %ErrFile% || goto :FinErr_NombreBD_Ed

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
call :LogScript "      4. BD sobre la que importar"
call :LogScript "      5. Descripción de la BD"
call :LogScript "      6. Borrar: 0/1. Indica si hay que borrar la BD en caso de que exista. Por defecto 0"
call :LogScript "      7. Fichero origen del import"
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
call :LogScriptPPAL "  IMPORT FINALIZADO ^(!Duracion!^)"
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
