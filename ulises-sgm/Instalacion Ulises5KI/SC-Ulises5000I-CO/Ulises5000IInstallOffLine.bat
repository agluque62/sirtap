@echo off

@REM Obtener el path desde donde se está ejecutando el Ulises5000I_Install.bat
set mi_path=%~d0%~p0

cls
@echo **************************************************************
@echo **** Utilidad de instalacion Servicios Web Ulises 5000-I  ****
@echo ****		       Configuracion OFFLINE                    ****
@echo **************************************************************
REM Iniciar servicio Web Deploy
@echo *** Iniciando servicio de instalacion web Offline**
net start "MsDepSvc"

:menu
cls
@echo **************************************************************
@echo **** Utilidad de instalacion Servicios Web Ulises 5000-I  ****
@echo ****            Configuracion OFFLINE                     ****
@echo **************************************************************
@echo.
@echo.
@echo Seleccione el servicio web a instalar
@echo Configuracion OffLine (C)
@echo Salir                 (S)
@echo.
choice /C CS /M "Pulse (C) o (S)" 
SET opcion=%errorlevel%

if errorlevel == 2 goto end
if errorlevel == 1 goto configuracion
goto menu

:configuracion

@echo **** Parando servicio IIS ******
iisreset /STOP

REM Crear directorio virtual "UlisesV5000OffLine"
@echo **************************************************************
@echo ****   Crear directorio virtual "UlisesV5000Offline"      ****
@echo **************************************************************
%windir%\System32\inetsrv\appcmd add app /site.name:"Default Web Site" /path:/UlisesV5000Offline /physicalPath:C:\inetpub\wwwroot\UlisesV5000Offline
%windir%\System32\inetsrv\appcmd set app /app.name: "Default Web Site/UlisesV5000Offline" /applicationPool:"ASP.NET v4.0"
%windir%\System32\inetsrv\appcmd set apppool "DefaultAppPool" /startMode:AlwaysRunning
%windir%\System32\inetsrv\appcmd set site "Default Web Site" /serverAutoStart:true

%windir%\System32\inetsrv\appcmd set app "Default Web Site/UlisesV5000Offline" /serviceAutoStartEnabled:true
%windir%\System32\inetsrv\appcmd set app "Default Web Site/UlisesV5000Offline/U5kCfg" /serviceAutoStartEnabled:true

%windir%\System32\inetsrv\appcmd set app "Default Web Site/UlisesV5000Offline/U5kCfg/Servicios" /serviceAutoStartEnabled:true

cls
IF /i NOT EXIST "C:\inetpub\wwwroot\UlisesV5000Offline" GOTO nobackup

@echo Desea conservar la configuracion WEB existente?
choice /C SN /M "Pulse (S), (N)" 
SET backup=%errorlevel%
if errorlevel 2 goto seguir
IF /i NOT EXIST "c:\tmpUlises" GOTO creadirtmp
rmdir /Q /S c:\tmpUlises

:creadirtmp
echo **** Backup Web.config
md c:\tmpUlises\web
md c:\tmpUlises\servicios

:UlisesV5000
move /Y C:\inetpub\wwwroot\UlisesV5000Offline\U5kCfg\Web.config c:\tmpUlises\web
move /Y C:\inetpub\wwwroot\UlisesV5000Offline\U5kCfg\Servicios\Web.config c:\tmpUlises\servicios
GOTO seguir

:nobackup
SET backup=2

:seguir
cls
@echo **** Instalando Servicio Web SCSM-Ulises5000 OffLine
cd %mi_path%
cd WebOffLine
call Web.deploy.cmd /y /m:localhost


pause

@echo **** Instalando Servicio Web Servicios
cd %mi_path%
cd ServiciosOffLine
call Servicios.deploy.cmd /y /m:localhost

pause

if %backup% == 2 goto cambiarpool
@echo **** Restore Web.config
move /Y c:\tmpUlises\web\Web.config C:\inetpub\wwwroot\UlisesV5000Offline\U5kCfg
move /Y c:\tmpUlises\servicios\Web.config C:\inetpub\wwwroot\UlisesV5000Offline\U5kCfg\Servicios
rmdir /Q /S c:\tmpUlises

:cambiarpool
REM Cambiar el application pool de UlisesV5000 ASP.NET v4.0
@echo *****************************************************************
@echo **** Cambiar el application pool de UlisesV5000 ASP.NET v4.0 ****
@echo *****************************************************************
%windir%\System32\inetsrv\appcmd set app /app.name: "Default Web Site/UlisesV5000Offline/U5kCfg" /applicationPool:"ASP.NET v4.0"
%windir%\System32\inetsrv\appcmd set app /app.name: "Default Web Site/UlisesV5000Offline/U5kCfg/Servicios" /applicationPool:"ASP.NET v4.0"

:fin
cls
@echo
@echo **** Iniciando servicio IIS ******
iisreset /START

@echo ****************************************************************************
@echo **** Directorio de apliación es                                         **** 
@echo **** C:\inetpub\wwwroot\UlisesV5000OffLine                              ****
@echo **** Recuerde asignar permiso Control Total sobre la carpeta            ****
@echo **** C:\inetpub\wwwroot\UlisesV5000OffLine a los grupos de Usuarios y   **** 
@echo **** Administradores.                                                   ****  
@echo ****************************************************************************

@echo ****************************************************************************
@echo *****       Fin de la Instalación Servicios Web Ulises 5000-I           ****
@echo *****                  Configuracion OFFLINE                            ****
@echo ****************************************************************************
@echo Pulse una tecla para finalizar
pause
goto menu

:end

cd %mi_path%
