@echo off

@REM Obtener el path desde donde se está ejecutando el Ulises5000I_Install.bat
set mi_path=%~d0%~p0

cls
@echo ******************************************************************************
@echo ****       Utilidad de instalacion Servicios Web Ulises 5000-I            ****
@echo ****                   Gestion Configuracion OFFLINE                      ****
@echo ******************************************************************************
REM Iniciar servicio Web Deploy
@echo ******************************************************************************
@echo ***                 Iniciando servicio de instalacion web                 ****
@echo ***                      Gestion Configuracion Offline                    ****
@echo ******************************************************************************
net start "MsDepSvc"

:menu
cls
@echo ******************************************************************************
@echo ****            Utilidad de instalacion Servicios Web Ulises 5000-I       ****
@echo ****                       Gestion Configuracion OFFLINE                  ****
@echo ******************************************************************************
@echo.
@echo.
@echo Seleccione el servicio web a instalar
@echo Gestion Configuracion OffLine     (G)
@echo Salir                             (S)
@echo.
choice /C GS /M "Pulse (G) o (S)" 
SET opcion=%errorlevel%

if errorlevel == 2 goto end
if errorlevel == 1 goto configuracion
goto menu

:configuracion

@echo **** Parando servicio IIS ******
iisreset /STOP

REM Crear directorio virtual "UlisesV5000GOffLine"
@echo ******************************************************************************
@echo ****            Crear directorio virtual "UlisesV5000GOffline"            ****
@echo ******************************************************************************

%windir%\System32\inetsrv\appcmd add app /site.name:"Default Web Site" /path:/UlisesV5000GOffline /physicalPath:C:\inetpub\wwwroot\UlisesV5000GOffline
%windir%\System32\inetsrv\appcmd set app /app.name: "Default Web Site/UlisesV5000GOffline" /applicationPool:"ASP.NET v4.0"
%windir%\System32\inetsrv\appcmd set apppool "DefaultAppPool" /startMode:AlwaysRunning
%windir%\System32\inetsrv\appcmd set site "Default Web Site" /serverAutoStart:true

%windir%\System32\inetsrv\appcmd set app "Default Web Site/UlisesV5000GOffline" /serviceAutoStartEnabled:true
%windir%\System32\inetsrv\appcmd set app "Default Web Site/UlisesV5000GOffline/U5kGO" /serviceAutoStartEnabled:true
%windir%\System32\inetsrv\appcmd set app "Default Web Site/UlisesV5000GOffline/U5kGO/Servicios" /serviceAutoStartEnabled:true

cls
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
move /Y C:\inetpub\wwwroot\UlisesV5000GOffline\U5kGO\Web.config c:\tmpUlises\web
move /Y C:\inetpub\wwwroot\UlisesV5000GOffline\U5kGO\Servicios\Web.config c:\tmpUlises\servicios

:seguir
cls
@echo ******************************************************************************
@echo ****        Instalando Servicio Web SCSM-Ulises5000 Gestion OffLine       ****
@echo ******************************************************************************
cd %mi_path%
cd Web
call Web.deploy.cmd /y /m:localhost

pause
@echo ******************************************************************************
@echo ****          Instalando Servicio Web Gestion Offline Servicios           **** 
@echo ******************************************************************************
cd %mi_path%
cd Servicios
call Servicios.deploy.cmd /y /m:localhost

pause

if %backup% == 2 goto cambiarpool
@echo **** Restore Web.config
move /Y c:\tmpUlises\web\Web.config C:\inetpub\wwwroot\UlisesV5000GOffline\U5kGO
move /Y c:\tmpUlises\servicios\Web.config C:\inetpub\wwwroot\UlisesV5000GOffline\U5kGO\Servicios
rmdir /Q /S c:\tmpUlises

:cambiarpool
REM Cambiar el application pool de UlisesV5000 ASP.NET v4.0
@echo ******************************************************************************
@echo ****          Cambiar el application pool de UlisesV5000 ASP.NET v4.0     ****
@echo ******************************************************************************
%windir%\System32\inetsrv\appcmd set app /app.name: "Default Web Site/UlisesV5000GOffline/U5kGO" /applicationPool:"ASP.NET v4.0"
%windir%\System32\inetsrv\appcmd set app /app.name: "Default Web Site/UlisesV5000GOffline/U5kGO/Servicios" /applicationPool:"ASP.NET v4.0"

:fin
@echocls
@echo **** Iniciando servicio IIS ******
iisreset /START
cls
@echo ******************************************************************************
@echo ****           Fin de la Instalacion Servicios Web Ulises 5000-I          **** 
@echo ****                    Gestion Configuracion OFFLINE                     ****
@echo ******************************************************************************
@echo ****                      Directorio de apliacion es                      ****
@echo ****                  C:\inetpub\wwwroot\UlisesV5000GOffLine              ****
@echo ****          Recuerde asignar permiso Control Total sobre la carpeta     ****
@echo **** C:\inetpub\wwwroot\UlisesV5000GOffLine a los grupos de Usuarios y    **** 
@echo **** Administradores.                                                     ****  
@echo ******************************************************************************
@echo Pulse una tecla para finalizar
pause
goto menu

:end

cd %mi_path%
