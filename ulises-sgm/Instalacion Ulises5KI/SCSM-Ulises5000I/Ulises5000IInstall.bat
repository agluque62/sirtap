@echo off

@REM Obtener el path desde donde se está ejecutando el Ulises5000I_Install.bat
set mi_path=%~d0%~p0

cls
@echo **************************************************************
@echo **** Utilidad de instalacion Servicios Web Ulises 5000-I  ****
@echo **************************************************************
REM Iniciar servicio Web Deploy
@echo *** Iniciando servicio de instalacion web **
net start "MsDepSvc"

:menu
cls
@echo **************************************************************
@echo **** Utilidad de instalacion Servicios Web Ulises 5000-I  ****
@echo **************************************************************
@echo.
@echo.
@echo Seleccione los servicios web a instalar
@echo Configuracion     (C)
@echo Base de Datos     (B)
@echo Salir             (S)
@echo.
choice /C CBS /M "Pulse (C), (B) o (S)" 
SET opcion=%errorlevel%

if errorlevel == 3 goto end
if errorlevel == 2 goto bd
if errorlevel == 1 goto configuracion
goto menu

:configuracion

@echo **** Parando servicio IIS ******
iisreset /STOP

REM Crear directorio virtual "UlisesV5000"
@echo **************************************************************
@echo **** Crear directorio virtual "UlisesV5000"
@echo **************************************************************
%windir%\System32\inetsrv\appcmd add app /site.name:"Default Web Site" /path:/UlisesV5000 /physicalPath:C:\inetpub\wwwroot\UlisesV5000
%windir%\System32\inetsrv\appcmd set app /app.name: "Default Web Site/UlisesV5000" /applicationPool:"ASP.NET v4.0"
%windir%\System32\inetsrv\appcmd set apppool "DefaultAppPool" /startMode:AlwaysRunning
%windir%\System32\inetsrv\appcmd set site "Default Web Site" /serverAutoStart:true

%windir%\System32\inetsrv\appcmd set app "Default Web Site/UlisesV5000" /serviceAutoStartEnabled:true
%windir%\System32\inetsrv\appcmd set app "Default Web Site/UlisesV5000/U5kCfg" /serviceAutoStartEnabled:true
%windir%\System32\inetsrv\appcmd set app "Default Web Site/UlisesV5000/U5kCfg/InterfazSacta" /serviceAutoStartEnabled:true
%windir%\System32\inetsrv\appcmd set app "Default Web Site/UlisesV5000/U5kCfg/InterfazSOAPConfiguracion" /serviceAutoStartEnabled:true
%windir%\System32\inetsrv\appcmd set app "Default Web Site/UlisesV5000/U5kCfg/Servicios" /serviceAutoStartEnabled:true

cls
@echo Desea conservar la configuración WEB existente?
choice /C SN /M "Pulse (S), (N)" 
SET backup=%errorlevel%

if errorlevel == 2 goto borra260
@echo **** Backup Web.config
@echo ****
REM Crear backup directorio aplicación "Nucleo"
@echo **************************************************************
@echo **** Seleccione version implementada actualmente:
@echo **** (A) Version superior a 2.6.0
@echo **** (K) Version igual o inferior a 2.6.0
@echo **************************************************************
choice /C AK /M "Pulse (A), (K)" 
SET origen=%errorlevel%
IF /i NOT EXIST "c:\tmpUlises" GOTO creadirtmp
rmdir /Q /S c:\tmpUlises
:creadirtmp
md c:\tmpUlises\web
md c:\tmpUlises\servicios
md c:\tmpUlises\isc
md c:\tmpUlises\mantto
md c:\tmpUlises\sacta

if %origen% == 2 goto nucleodf
if %origen% == 1 goto UlisesV5000

:UlisesV5000
move /Y C:\inetpub\wwwroot\UlisesV5000\U5kCfg\Web.config c:\tmpUlises\web
move /Y C:\inetpub\wwwroot\UlisesV5000\U5kCfg\Servicios\Web.config c:\tmpUlises\servicios
move /Y C:\inetpub\wwwroot\UlisesV5000\U5kCfg\InterfazSOAPConfiguracion\Web.config c:\tmpUlises\isc
move /Y C:\inetpub\wwwroot\UlisesV5000\U5kCfg\InterfazSacta\Web.config c:\tmpUlises\sacta
goto borra260

:nucleodf
move /Y C:\inetpub\wwwroot\NucleoDF\U5kCfg\Web.config c:\tmpUlises\web
move /Y C:\inetpub\wwwroot\NucleoDF\U5kCfg\Servicios\Web.config c:\tmpUlises\servicios
move /Y C:\inetpub\wwwroot\NucleoDF\U5kCfg\InterfazSOAPConfiguracion\Web.config c:\tmpUlises\isc
move /Y C:\inetpub\wwwroot\NucleoDF\U5kCfg\InterfazSacta\Web.config c:\tmpUlises\sacta

:borra260
IF /i NOT EXIST "c:\inetpub\wwwroot\NucleoDF" GOTO seguir
@echo **************************************************************
@echo **** Desinstalacion Web 2.5.x a 2.6.0
@echo **************************************************************

"C:\Program Files (x86)\IIS\Microsoft Web Deploy V3\msdeploy" -verb:delete -dest:apphostconfig="Default Web Site/NucleoDF/U5kCfg/InterfazSacta"
"C:\Program Files (x86)\IIS\Microsoft Web Deploy V3\msdeploy" -verb:delete -dest:apphostconfig="Default Web Site/NucleoDF/U5kCfg/InterfazSOAPConfiguracion"
"C:\Program Files (x86)\IIS\Microsoft Web Deploy V3\msdeploy" -verb:delete -dest:apphostconfig="Default Web Site/NucleoDF/U5kCfg/Servicios"
"C:\Program Files (x86)\IIS\Microsoft Web Deploy V3\msdeploy" -verb:delete -dest:apphostconfig="Default Web Site/NucleoDF/U5kCfg"
"C:\Program Files (x86)\IIS\Microsoft Web Deploy V3\msdeploy" -verb:delete -dest:apphostconfig="Default Web Site/NucleoDF"
pause

:seguir
cls
@echo **** Instalando Servicio Web SCSM-Ulises5000
cd %mi_path%
cd Web
call Web.deploy.cmd /y /m:localhost

pause

@echo **** Instalando Servicio Web InterfazSacta
cd %mi_path%
cd InterfazSacta
call InterfazSacta.deploy.cmd /y /m:localhost

pause 

@echo **** Instalando Servicio Web InterfazSOAPConfiguracion
cd %mi_path%
cd InterfazSOAPConfiguracion
call InterfazSOAPConfiguracion.deploy.cmd /y /m:localhost

pause

@echo **** Instalando Servicio Web Servicios
cd %mi_path%
cd Servicios
call Servicios.deploy.cmd /y /m:localhost

pause

if %backup% == 2 goto cambiarpool
@echo **** Restore Web.config
move /Y c:\tmpUlises\web\Web.config C:\inetpub\wwwroot\UlisesV5000\U5kCfg
move /Y c:\tmpUlises\servicios\Web.config C:\inetpub\wwwroot\UlisesV5000\U5kCfg\Servicios
move /Y c:\tmpUlises\isc\Web.config C:\inetpub\wwwroot\UlisesV5000\U5kCfg\InterfazSOAPConfiguracion
move /Y c:\tmpUlises\sacta\Web.config C:\inetpub\wwwroot\UlisesV5000\U5kCfg\InterfazSacta
rmdir /Q /S c:\tmpUlises

if %origen% == 1 goto cambiarpool
REM Reemplazar el identificador del directorio NucleoDF por el de UlisesV5000 en los ficheros de configuración restaurados de versiones anteriores a V2.6.1
@echo **********************************************************************************
@echo **** Reemplazando nombre de directorio NucleoDF por UlisesV5000 en *\Web.config´s
@echo **********************************************************************************
powershell -Command "(gc C:\inetpub\wwwroot\UlisesV5000\U5kCfg\Web.config) -replace 'NucleoDF', 'UlisesV5000' | Out-File C:\inetpub\wwwroot\UlisesV5000\U5kCfg\Web.config"
powershell -Command "(gc C:\inetpub\wwwroot\UlisesV5000\U5kCfg\Servicios\Web.config) -replace 'NucleoDF', 'UlisesV5000' | Out-File C:\inetpub\wwwroot\UlisesV5000\U5kCfg\Servicios\Web.config"
powershell -Command "(gc C:\inetpub\wwwroot\UlisesV5000\U5kCfg\InterfazSacta\Web.config) -replace 'NucleoDF', 'UlisesV5000' | Out-File C:\inetpub\wwwroot\UlisesV5000\U5kCfg\InterfazSacta\Web.config"
powershell -Command "(gc C:\inetpub\wwwroot\UlisesV5000\U5kCfg\InterfazSOAPConfiguracion\Web.config) -replace 'NucleoDF', 'UlisesV5000' | Out-File C:\inetpub\wwwroot\UlisesV5000\U5kCfg\InterfazSOAPConfiguracion\Web.config"


:cambiarpool
REM Cambiar el application pool de UlisesV5000 ASP.NET v4.0
@echo **************************************************************
@echo **** Cambiar el application pool de UlisesV5000 ASP.NET v4.0
@echo **************************************************************
%windir%\System32\inetsrv\appcmd set app /app.name: "Default Web Site/UlisesV5000/U5kCfg" /applicationPool:"ASP.NET v4.0"
%windir%\System32\inetsrv\appcmd set app /app.name: "Default Web Site/UlisesV5000/U5kCfg/InterfazSacta" /applicationPool:"ASP.NET v4.0"
%windir%\System32\inetsrv\appcmd set app /app.name: "Default Web Site/UlisesV5000/U5kCfg/InterfazSOAPConfiguracion" /applicationPool:"ASP.NET v4.0"
%windir%\System32\inetsrv\appcmd set app /app.name: "Default Web Site/UlisesV5000/U5kCfg/Servicios" /applicationPool:"ASP.NET v4.0"

REM Instalar claves de registro
cls
@echo **************************************************************
@echo **** Instalar claves de registro
@echo **************************************************************
cd \inetpub\wwwroot\UlisesV5000\U5kCfg\Servicios
trans.reg

:fin
cls
@echo **** Iniciando servicio IIS ******
iisreset /START


@echo **************************************************************
@echo **** Directorio de apliación es C:\inetpub\wwwroot\UlisesV5000
@echo **** Recuerde asignar permiso Control Total sobre la carpeta
@echo **** C:\inetpub\wwwroot\UlisesV5000 a los grupos de Usuarios y 
@echo **** Administradores.  
@echo **************************************************************
@echo *****
@echo *****  Fin de la Instalación Servicios Web Ulises 5000-I *****
@echo Pulse una tecla para finalizar
pause
goto menu

:bd
REM Instalar base de datos Ulises
cls
@echo **************************************************************
@echo **** Crear base de datos Ulises
@echo **************************************************************
cls
@echo  ¿Desea crear o actualizar la base de datos Ulises?
choice /C SN /M "Pulse (S), (N)" 

if errorlevel == 2 goto end
cd %mi_path%
cd "Actualizacion BD"
call "BDCD40 Installation.bat"
goto menu

:end

cd %mi_path%
