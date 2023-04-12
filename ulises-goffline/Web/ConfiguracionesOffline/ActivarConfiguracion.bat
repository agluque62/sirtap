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
set TituloProceso=Activar Conf. SCV %date% %time%
title %TituloProceso%
REM ----------------------------------------------------------------------------
REM    Parámetros del script:
REM     1. GUID del proceso
REM     2. Usuario de la BD
REM     3. Password del usuario
REM     4. Nombre de la Configuración a pasar a Online (new_cd40_trans)
REM     5. Servidor de BD
REM ----------------------------------------------------------------------------
for /F "tokens=*" %%L in ('hostname') do set Servidor=%%L
for /F "tokens=*" %%L in ('cd') do set DirTrabajo=%%L
if /I "%ProcesoPPAL%" == "" (set ProcesoPPAL=%0)
set Codigo_Error=ERR_ACT
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
Set BD_CD40_Offline=%4
if NOT [%5] == [] set Servidor=%5
if [%5] == [] set Servidor=LOCALHOST

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
call :LogScriptPPAL "  SCRIPT PARA ACTIVAR UNA CONF. OFFLINE SCV"
call :LogScriptPPAL "__________________________________________________________________________"
call :LogScript ""

REM PID del proceso
REM En lugar de usar el PID para registrar el log del proceso se pasará a usar el GUID que se pasa como primer parámetro del script
REM tasklist /fi "imagename eq cmd.exe" /v  > %LogTempFile%
REM for /F "tokens=1,2,*" %%a in ('findstr /irc:".*%TituloProceso%" %LogTempFile%') do  set PIDComando=%%b

call :LogScript "   Activar conf. offline %BD_CD40_Offline%:"
call :LogScript "      Usuario      : %Usuario%"
call :LogScript "      Servidor     : %Servidor%"
if NOT [%ServidorLogProcesos%] == [%Servidor%] (
  call :LogScript "      ServidorLog  : %ServidorLogProcesos%"
)

call :AddLogProceso "Activando conf. %BD_CD40_Offline% ..." || goto :FIN_ERROR

call :UpdLogProceso "Comprobando conf. %BD_CD40_Offline%" || goto :FIN_ERROR
call :CheckBD_Existe %BD_CD40_Offline% || goto :FIN_ERROR

call :NombreBD_Ed %BD_CD40_Offline% || goto :FIN_ERROR
set BD_CD40_Offline_Ed=%BD_Ed%

call :CheckBD_Existe %BD_CD40_Offline_Ed% || goto :FIN_ERROR

call :NombreBD_Ed_Online || goto :FIN_ERROR
set BD_CD40_Online_Ed=%BD_Online_Ed%

call :LogScript ""
call :LogScript "      BD_CD40_Offline   : %BD_CD40_Offline%"
call :LogScript "      BD_CD40_Offline_Ed: %BD_CD40_Offline_Ed%"
call :LogScript "      BD_CD40_Online_Ed : %BD_CD40_Online_Ed%"
call :LogScript ""

:COMPROBAR_ESQUEMA
call :UpdLogProceso "Comprobando esquema %BD_CD40_Offline_Ed%" || goto :FIN_ERROR
call :LogScript ""
call :LogScript "   Comprobar que ambas BD de ed (_trans) tienen el mismo esquema"
mysql --defaults-file=%InfoConexion% -h %Servidor% -f -n -N -B -e "SELECT new_cd40_offline.DiffEsquemaConfEdicion('%BD_CD40_Offline_Ed%');" > %SQLTempFile% 2> %ErrFile%
call :ComprobarErrores "Comprobando el esquema de %BD_CD40_Offline_Ed%" "%ErrFile%" || goto :FIN_ERROR

REM El log no debe tener ni una sola línea
set /a NumDiff=0
for /F "tokens=*" %%L in (%SQLTempFile%) do (
   set /a NumDiff+=1
)

if %NumDiff% NEQ 0 (
  call :LogError "El esquema de %BD_CD40_Offline_Ed% es diferente al de ed. de prod. Consultar el fichero %SQLTempFile% para ver las diferencias"
  goto :FIN_ERROR 
)

:EXPORT_TRIGGERS
call :UpdLogProceso "Exportando triggers de %BD_CD40_Online_Ed%" || goto :FIN_ERROR
call :LogScript ""
call :LogScript "   Exportar los triggers de %BD_CD40_Online_Ed%"
REM Exportar los triggers de la BD de producción, para importarla después de cambiar los datos de la BD de edición
call ExportarConfiguracion.bat %GUID% %Usuario% %Password% %BD_CD40_Online_Ed% "-- Version temporal %ProcesoActual%" TRIGGERS "%DumpTempFile%" %Servidor% 2> %ErrFile%
call :ComprobarErrores "Exportando los TRIGGERS de %BD_CD40_Online_Ed%" "%ErrFile%" || goto :FIN_ERROR

:BORRAR_TRIGGERS
call :UpdLogProceso "Borrando triggers de %BD_CD40_Online_Ed%" || goto :FIN_ERROR
call :LogScript ""
call :LogScript "   Borrar triggers de %BD_CD40_Online_Ed%"
REM Los triggers no se puede borrar desde un proced. almacenado, habrá que hacerlo desde aquí
REM mysql --defaults-file=%InfoConexion% -h %Servidor% -f -n -N -B -e "CALL new_cd40_offline.DropTriggers('%BD_CD40_Online_Ed%')" > %SQLTempFile% 2> %ErrFile%
echo.USE %BD_CD40_Online_Ed%; > %SQLTempFile%
mysql --defaults-file=%InfoConexion% -h %Servidor% -f -n -N -B -e "SELECT Concat('DROP TRIGGER ', TRIGGER_NAME, ';') FROM  information_schema.TRIGGERS WHERE TRIGGER_SCHEMA = '%BD_CD40_Online_Ed%';" >> %SQLTempFile% 2> %ErrFile%
call :ComprobarErrores "Creando script para borrar los TRIGGERS de %BD_CD40_Online_Ed%" "%ErrFile%" || goto :FIN_ERROR

mysql --defaults-file=%InfoConexion% -h %Servidor% -f -n -N -B -e "source "%SQLTempFile%";" 2> %ErrFile% 
call :ComprobarErrores "Borrando los TRIGGERS de %BD_CD40_Online_Ed%" "%ErrFile%" || goto :FIN_ERROR

:COPIAR_DATOS
call :UpdLogProceso "Copiando conf. %BD_CD40_Offline_Ed% en %BD_CD40_Online_Ed%" || goto :FIN_ERROR
call :LogScript ""
call :LogScript "   Copiar conf. %BD_CD40_Offline_Ed% a prod."
mysql --defaults-file=%InfoConexion% -h %Servidor% -f -n -N -B -e "CALL new_cd40_offline.ActivarConfiguracion('%BD_CD40_Offline_Ed%')" > %SQLTempFile% 2> %ErrFile%
call :ComprobarErrores "Copiando los datos de la conf. offline a prod. ^(BD _trans^)" "%ErrFile%" || goto :FIN_ERROR

REM Rellenar TablasModificadas
call :UpdLogProceso "Rellenando lista de TablasModificadas en %BD_CD40_Online_Ed%" || goto :FIN_ERROR
mysql --defaults-file=%InfoConexion% -h %Servidor% -f -n -N -B -e "CALL new_cd40_offline.InsertarTablasModificadas('%BD_CD40_Online_Ed%')" > %SQLTempFile% 2> %ErrFile%
call :ComprobarErrores "Rellenando la lista de TablasModificadas en %BD_CD40_Online_Ed%" "%ErrFile%" || goto :FIN_ERROR

:IMPORT_TRIGGERS
call :UpdLogProceso "Importando triggers en %BD_CD40_Online_Ed%" || goto :FIN_ERROR
REM Solo hay que importar el código de la BD de edición, no toda la configuración, así que no se usa ImportarConfiguracion.bat
call :LogScript ""
call :LogScript "   Importar los triggers de %BD_CD40_Online_Ed%"
mysql --defaults-file=%InfoConexion% -h %Servidor% -c -e "source "%DumpTempFile%";" 2> %ErrFile% 
call :ComprobarErrores "Importando los triggers en %BD_CD40_Offline_Ed%" "%ErrFile%" || goto :FIN_ERROR

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
REM    powershell -ExecutionPolicy Bypass -File .\LogProceso.ps1 -UsuarioBD "%Usuario%" -PasswordBD "%Password%" -ServidorBD "%ServidorLogProcesos%"
 -GUID "%GUID%" -Descripcion "%~1" 2> %ErrLogProceso%
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
REM CheckBD_Existe: comprueba si existe la BD
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

  if %NumBD% NEQ 1 (
    call :LogScript ""
    call :LogScript "   ** La BD %BD% ya está en la lista de CONFIGURACIONES"
  )

  REM Consultamos si la BD / Configuración existe como BD
  mysql --defaults-file=%InfoConexion% -h %Servidor% -f -n -N -B -e "SELECT SCHEMA_NAME FROM INFORMATION_SCHEMA.SCHEMATA WHERE SCHEMA_NAME = '%BD%'" > %SQLTempFile% 2> %ErrFile%
  call :ComprobarErrores "Consultando la BD %BD% en la lista de esquemas del servidor" "%ErrFile%" || goto :FinErr_CheckBD_Existe

  REM El log debe tener 1 líneas con el nombre de la BD
  set /a NumBD=0
  for /F "tokens=*" %%L in (%SQLTempFile%) do (
     set /a NumBD+=1
  )

  if %NumBD% NEQ 1 (
    call :LogError "La BD %BD% no existe en el servidor "
	goto :FinErr_CheckBD_Existe
  )

:Fin_CheckBD_Existe
  goto :eof
:FinErr_CheckBD_Existe
  EXIT /B 1

REM ---------------------------------------------------------------------------
REM NombreBD_Ed: Consulta el nombre que le corresponde a la BD de Edición
REM ---------------------------------------------------------------------------
:NombreBD_Ed
  set BD=%~1
  REM Obtener el nombre que le corresponde a la BD de edición
  mysql --defaults-file=%InfoConexion% -h %Servidor% -f -n -N -B -e "SELECT new_cd40_offline.GetNombreConfEdicion('%BD%')" > %SQLTempFile% 2> %ErrFile%
  call :ComprobarErrores "Consultando el nombre de la BD de ed. ^(_trans^) de %BD% " "%ErrFile%" || goto :FinErr_NombreBD_Ed

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
REM NombreBD_Ed_Online
REM Consulta el nombre de la BD Online de edición (trans)
REM ---------------------------------------------------------------------------
:NombreBD_Ed_Online
  mysql --defaults-file=%InfoConexion% -h %Servidor% -f -n -N -B -e "SELECT CONF_EDICION FROM new_cd40_offline.CONFIGURACIONES WHERE TIPO = 'ONLINE'" > %SQLTempFile%
  call :ComprobarErrores "Consultando el nombre de la BD Online" "%ErrFile%" || goto :FinErr_NombreBD_Ed_Online
  
  REM El log incluirá el nombre que le corresponde a la BD Edicion
  set /a NumBD=0
  for /F "tokens=*" %%L in (%SQLTempFile%) do (
     set /a NumBD+=1
	 set BD_Online_Ed=%%L
  )

  if %NumBD% NEQ 1 (
    call :LogError "No se encuentra ninguna BD configurada como Online"
	goto :FinErr_NombreBD_Ed_Online
  )

:Fin_NombreBD_Ed_Online
  goto :eof
:FinErr_NombreBD_Ed_Online
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
call :LogScript "      4. Nombre de la Conf. a pasar a Online ^(_trans^)"
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
call :LogScriptPPAL "  CONF. ACTIVADA ^(!Duracion!^)"
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
